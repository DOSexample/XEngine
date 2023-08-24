#include "GXDMotionLoader.h"
#include "FileLoader.h"

#include "../XDevice.h"

#include "../XSystem/IO/BinaryReader.h"
#include "../XSystem/IO/Zlib.h"
using namespace XSystem::IO;

#include "../XAnimation/XAnimationMotion.h"

/// <summary>
/// LoadMotion2Base
/// </summary>
namespace {

	struct MotionKeyFrame
	{
		D3DXQUATERNION rotation;
		D3DXVECTOR3 position;
	};

	bool LoadMotion2Extra(BinaryReader& br, bool& tValid, bool& tCompressed)
	{
		char tBuffer[4];
		return br.ReadBytes(tBuffer, sizeof(tBuffer)) && (tValid = tBuffer[0] > 0, tCompressed = tBuffer[1] > 0);
	}
	bool LoadMotion2Header(BinaryReader& br, int* tVersion)
	{
		char tBuffer[12];
		memset(tBuffer, 0, sizeof(tBuffer));

		char* tmp = br.ReadString(tBuffer, 7);
		if (!tmp)
			return false;

		if (strncmp("MOTION", tBuffer, 6) == 0)
		{
			*tVersion = tBuffer[6] - '0';
			if (*tVersion == 2 || *tVersion == 3)
				return true;
		}
		else
		{
			//set offset back to 0
			br.SetCurrentOffset(0);
			*tVersion = 1;
			return true;
		}

		return false;
	}

	bool ApplyLoadMotion(XAnimationMotion* s, BinaryReader& br)
	{
		std::vector<MotionKeyFrame> mMotionKey;

		s->mFrameNum = br.ReadInt();
		s->mBoneNum = br.ReadInt();
		printf("Frame: %d, Bone: %d\n", s->mFrameNum, s->mBoneNum);

		int totalKeySize = s->mFrameNum * s->mBoneNum;
		if ( totalKeySize > 0 )
		{
			mMotionKey.resize(totalKeySize);
			br.ReadBytes(mMotionKey.data(), totalKeySize * sizeof(MotionKeyFrame));

			for (int i = 0; i < s->mFrameNum; ++i)
			{
				std::vector<D3DXMATRIX> v = {};

				for (int j = 0; j < s->mBoneNum; ++j)
				{
					MotionKeyFrame* r = &mMotionKey[i * s->mBoneNum + j];
					//auto mx = new D3DXMATRIX();
					D3DXMATRIX mx;
					D3DXMatrixRotationQuaternion(&mx, (const D3DXQUATERNION*)&r->rotation);
					mx._41 = r->position.x;
					mx._42 = r->position.y;
					mx._43 = r->position.z;
					v.push_back( mx );
				}

				s->mMatrixKey.insert({i, v});
			}

		}
		return totalKeySize > 0 && mMotionKey.size() == totalKeySize;
	}


	bool LoadMotion2UncompressedChunk(XAnimationMotion* s, BinaryReader& br)
	{
		return ApplyLoadMotion(s, br);
	}

	bool LoadMotion2CompressChunk(XAnimationMotion* s, BinaryReader& br)
	{
		Zlib z(br);
		if (!Zlib::Decompress(z))
		{
			return false;
		}

		BinaryReader sub(&z.m_OriginalData, 0, z.m_OriginalSize);
		return LoadMotion2UncompressedChunk(s, sub);
	}
}

namespace XLoader
{
	GXDMotionLoader::GXDMotionLoader()
	{
	}

	bool GXDMotionLoader::Load(const char* tFileName, XAnimationMotion* anim)
	{
		std::vector<char> data;
		FileLoader f;
		if (!f.Load(tFileName, data))
			return false;

		BinaryReader br(&data, 0, data.size());

		bool tValid = false, tCompressed = false;
		bool result = false;
		int tVersion;
		if( LoadMotion2Header(br, &tVersion) )
		{
			switch ( tVersion )
			{
			case 1://TwelveSky1 | TwelveSky2
				tValid = true;
				tCompressed = true;
				break;
			case 2://Troy vs Sparta | Waren Story
				tValid = true;
				break;
			case 3://TwelveSky2.5
				LoadMotion2Extra(br, tValid, tCompressed);
				break;
			}

			if (tValid)
			{
				if (tCompressed) {
					result = LoadMotion2CompressChunk(anim, br);
				}
				else {
					result = LoadMotion2UncompressedChunk(anim, br);
				}
				anim->Create(result);
			}
		}

		return result;
	}
}
