#include "GXDSkin2Loader.h"
#include "GXDTextureLoader.h"

#include "FileLoader.h"
#include "../XSystem/IO/Zlib.h"
#include "../XSystem/Encryption/XXTea.h"

#include "../XEngine.h"
#include "../XSkinMesh.h"

using namespace XSystem::IO;
using namespace XSystem::Encryption;

void DebugA(const char* _Format,...)
{
	char _Buffer[4096];
	int _Result;
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);

	_Result = _vsprintf_l(_Buffer, _Format, NULL, _ArgList);

	__crt_va_end(_ArgList);
	printf(_Buffer);
}

/// <summary>
/// LoadSkin2Data
/// </summary>
namespace {

	bool LoadSkin2Texture(BinaryReader& br, SkinData2& s)
	{
		if (!s.mDiffuseMap.Load(br))
		{
			DebugA("<< !LoadSkin2Texture::mDiffuseMap()\r\n");
			return false;
		}

		if (!s.mNormalMap.Load(br))
		{
			DebugA("<< !LoadSkin2Texture::mNormalMap()\r\n");
			return false;
		}

		if (!s.mSpecularMap.Load(br))
		{
			DebugA("<< !LoadSkin2Texture::mSpecularMap()\r\n");
			return false;
		}
		if (s.mRequireAlbedoMap && !s.mAlbedoMap.Load(br))
		{
			DebugA("<< !LoadSkin2Texture::mAlbedoMap()\r\n");
			return false;
		}

		int mAnimationNum = br.ReadInt();
		if (mAnimationNum > 0)
		{
			DebugA("<< LoadSkin2Texture::mAnimationNum() -> %d\r\n", mAnimationNum);
			s.mAnimationMap.resize(mAnimationNum);
			for (int i = 0; i < mAnimationNum; ++i)
				if (!s.mAnimationMap[i].Load(br))
					return false;
		}

		DebugA("<< LoadSkin2Texture()\r\n");

		return true;
	}

	bool LoadSkin2Vertex(BinaryReader& br, SkinData2& s)
	{
		int mLODStepNum = 0;
		if (!br.ReadBytes(&mLODStepNum, sizeof(mLODStepNum)))
		{
			DebugA("<< !LoadSkin2Vertex::mLODStepNum -> %d\r\n", mLODStepNum);
			return false;
		}

		DebugA("<< LoadSkin2Vertex::mLODStepNum -> %d\r\n", mLODStepNum);

		if (!mLODStepNum)
			return false;

		s.mLOD.resize( mLODStepNum );

		//auto device = &XEngine::Instance();
		for (int i = 0; i < mLODStepNum; ++i)
		{
			auto l = &s.mLOD[i];

			int mVertexNum = l->mVertexNum = br.ReadInt();
			int vtxSize = sizeof(SkinVertex2) * mVertexNum;
			auto vtx = &l->vertices;
			//void* vbuffer = NULL;
			//device->CreateVertexBuffer(vtxSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &vtx, 0);
			//vtx->Lock(0, 0, &vbuffer, 0);
			//if (br.ReadBytes(vbuffer, vtxSize)) {
			//}
			//vtx->Unlock();
			vtx->resize( mVertexNum );
			br.ReadBytes( vtx->data(), vtxSize );


			int mIndexNum = l->mIndexNum = br.ReadInt();
			int idxSize = 6 * mIndexNum;
			auto idx = &l->indices;
			//void* ibuffer = NULL;
			//device->CreateIndexBuffer(idxSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &idx, 0);
			//idx->Lock(0, 0, &ibuffer, 0);
			//if (br.ReadBytes(ibuffer, idxSize)) {
			//}
			//idx->Unlock();
			//br.ReadEmpty(idxSize);
			idx->resize(mIndexNum * 3);
			br.ReadBytes( idx->data(), idxSize );


			int shdSize = sizeof(SkinShadow2) * mVertexNum;

			//mShadowVertexBuffer
			br.ReadEmpty(shdSize);

			//mShadowIndexBuffer
			br.ReadEmpty(idxSize);

			//mShadowEdgeBuffer
			br.ReadEmpty(idxSize);
		}

		return LoadSkin2Texture(br, s);
	}

	bool LoadSkinData2(BinaryReader& br, SkinData2& s)
	{
		s.mCheckValidState = br.ReadInt() > 0;
		if (!s.mCheckValidState)
		{
			return true;
		}

		s.mCheckValidState = false;

		if (!br.ReadBytes(&s.mEffect, sizeof(s.mEffect)))
		{
			DebugA("<< !LoadSkin2Real::mEffect()\r\n");
			return false;
		}
		
		if (!br.ReadBytes(&s.mSize, sizeof(s.mSize)))
		{
			DebugA("<< !LoadSkin2Real::mSize()\r\n");
			return false;
		}
		
		if (!br.ReadBytes(s.mVertexBufferForBillboard, sizeof(s.mVertexBufferForBillboard)))
		{
			DebugA("<< !LoadSkin2Real::mVertexBufferForBillboard()\r\n");
			return false;
		}

		s.mCheckValidState = LoadSkin2Vertex(br, s);
		DebugA("<< LoadSkin2Real::LoadSkin2Vertex() -> %d\r\n", s.mCheckValidState);

		return s.mCheckValidState;
	}
}

/// <summary>
/// LoadSkin2Base
/// </summary>
namespace {

	bool LoadSkin2Extra(BinaryReader& br, bool& tValid, char& tCompressed)
	{
		char tBuffer[4];
		char* tmp = br.ReadBytes( tBuffer, sizeof(tBuffer) );

		if (!tmp)
			return false;

		tValid = tBuffer[0] > 0;
		tCompressed = tBuffer[1];

		return true;
	}
	bool LoadSkin2Header(BinaryReader& br, int& tVersion, bool& isXXTEA)
	{
		isXXTEA = false;
		char tBuffer[12];
		memset(tBuffer, 0, sizeof(tBuffer));

		char* tmp = br.ReadString(tBuffer, 8);
		if (!tmp)
			return false;

		if (strncmp("SOBJECT", tBuffer, 7) == 0)
		{
			tVersion = tBuffer[7] - '0';
			if (tVersion == 2 || tVersion == 3)
				return true;
		}
		else if (strncmp("XXTEA111", tBuffer, 8) == 0)
		{
			tVersion = 4;
			isXXTEA = true;
			return true;
		}
		else if (strncmp("XXTEA222", tBuffer, 8) == 0)
		{
			tVersion = 5;
			isXXTEA = true;
			return true;
		}

		return false;
	}

	bool CheckXXTEA(BinaryReader& br)
	{
		unsigned int xxteaData[2] = { br.ReadUInt(), br.ReadUInt() };
		if (XXTea::Excute(XXTea::Type::Decrypt, xxteaData, 2))
		{
			int tOriginalSize = (int)xxteaData[0];
			int tCompressSize = (int)xxteaData[1];

			size_t offset = br.GetCurrentOffset() - 8;
			//set offset back to current-8
			br.SetCurrentOffset( offset );

			//set buffer to tOriginalSize and tCompressSize
			br.SetBuffer( &tOriginalSize, sizeof(int), offset );
			br.SetBuffer( &tCompressSize, sizeof(int), offset+4 );

			printf("%d : %d\n", tOriginalSize, tCompressSize);

			return true;
		}

		return false;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="s"></param>
	/// <param name="br"></param>
	/// <param name="isXXTEA"> = for XXTEA</param>
	/// <param name="tRequireAlbedoMap"> = for XXTEA222</param>
	/// <returns></returns>
	bool LoadSkin2UncompressedChunk(SkinVersion2* s, BinaryReader& br, bool isXXTEA = false, bool tRequireAlbedoMap = false)
	{
		if (isXXTEA && !CheckXXTEA(br))
			return false;

		bool result = false;
		int mSkinNum = br.ReadInt();
		if (mSkinNum > 0)
		{
			s->mSkin.resize(mSkinNum);
			for (int i = 0; i < mSkinNum; i++)
			{
				s->mSkin[i].mRequireAlbedoMap = tRequireAlbedoMap;
				result = LoadSkinData2(br, s->mSkin[i]);
				if (!result)
					break;
			}

			DebugA("<< LoadSkin2UncompressedChunk::LoadSkin2Data() -> %d\r\n", result);
		}

		return result;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="s"></param>
	/// <param name="br"></param>
	/// <param name="isXXTEA"> = for XXTEA111 or XXTEA222</param>
	/// <param name="tRequireAlbedoMap"> = for XXTEA222</param>
	/// <returns></returns>
	bool LoadSkin2CompressChunk(SkinVersion2* s, BinaryReader& br, bool isXXTEA = false, bool tRequireAlbedoMap = false)
	{
		if (isXXTEA && !CheckXXTEA(br))
			return false;

		Zlib z( br );
		if (!Zlib::Decompress(z))
		{
			DebugA("<< !LoadSkin2CompressChunk::Decompress()\r\n");
			return false;
		}

		bool result = false;
		BinaryReader sub( &z.m_OriginalData, 0, z.m_OriginalSize );
		int mSkinNum = sub.ReadInt();
		if (mSkinNum > 0)
		{
			s->mSkin.resize(mSkinNum);
			for (int i = 0; i < mSkinNum; i++)
			{
				s->mSkin[i].mRequireAlbedoMap = tRequireAlbedoMap;
				result = LoadSkinData2(sub, s->mSkin[i]);
				if (!result)
					break;
			}
		}
		DebugA("<< LoadSkin2CompressChunk::LoadSkin2Data() -> %d\r\n", result);

		return result;
	}


}

namespace XLoader {

	GXDSkin2Loader::GXDSkin2Loader()
	{
	}

	bool GXDSkin2Loader::Load(const char* tFileName, XSkinMesh* skin)
	{
		bool result = false;
		FileLoader f;
		std::vector<char> data;

		if (!f.Load(tFileName, data))
		{
			DebugA( "%s << size: %d\r\n", tFileName, data.size() );
			return false;
		}
		
		int tVersion;
		bool tValid = false;
		char tCompressed = '0';
		bool tHasAlbedo = false;
		bool isXXTEA = false;

		BinaryReader br( &data, 0, data.size() );
		if (!LoadSkin2Header(br, tVersion, isXXTEA))
		{
			DebugA("%s << !LoadSkin2Header()\r\n", tFileName);
			return result;
		}

		switch ( tVersion )
		{
		case 2://Troy vs Sparta | Waren Story
			tValid = true;
			break;
		case 3://TwelveSky2.5
			if (!LoadSkin2Extra(br, tValid, tCompressed))
			{
				DebugA("%s << !LoadSkin2Extra()\r\n", tFileName);
			}
			break;
		case 4://TwelveSky2GXCW - XXTEA111
			if (!LoadSkin2Extra(br, tValid, tCompressed))
			{
				DebugA("%s << !LoadSkin2Extra()\r\n", tFileName);
				break;
			}
			break;
		case 5://TwelveSky2GXCW - XXTEA222 (Albedo Texture)
			if (!LoadSkin2Extra(br, tValid, tCompressed))
			{
				DebugA("%s << !LoadSkin2Extra()\r\n", tFileName);
				break;
			}
			tHasAlbedo = true;
			break;
		}

		if (tValid)
		{
			skin->v2 = new SkinVersion2();
			if (tCompressed != '0') {
				result = LoadSkin2CompressChunk(skin->v2, br, isXXTEA, tHasAlbedo);
			}
			else {
				result = LoadSkin2UncompressedChunk(skin->v2, br, isXXTEA, tHasAlbedo);
			}
			skin->Create2(result);

			if (!result) {
				delete skin->v2;
				skin->v2 = nullptr;
			}
		}

		return result;
	}

}