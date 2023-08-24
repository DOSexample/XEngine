#include "GXDSkin2Loader.h"
#include "GXDTextureLoader.h"

#include "FileLoader.h"
#include "../XSystem/IO/Zlib.h"
#include "../XSystem/Encryption/XXTea.h"

#include "../XEngine.h"
#include "../XSkinMesh.h"

#include <string>

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

namespace {

	bool CheckXXTEA(BinaryReader& br)
	{
		unsigned int xxteaData[2] = { br.ReadUInt(), br.ReadUInt() };
		if (XXTea::Excute(XXTea::Type::Decrypt, xxteaData, 2))
		{
			int tOriginalSize = (int)xxteaData[0];
			int tCompressSize = (int)xxteaData[1];

			size_t offset = br.GetCurrentOffset() - 8;
			//set offset back to current-8
			br.SetCurrentOffset(offset);

			//set buffer to tOriginalSize and tCompressSize
			br.SetBuffer(&tOriginalSize, sizeof(int), offset);
			br.SetBuffer(&tCompressSize, sizeof(int), offset + 4);

			printf("%d : %d\n", tOriginalSize, tCompressSize);

			return true;
		}

		return false;
	}

	bool LoadSkinHeader(BinaryReader& br, float& tVersion, bool& isXXTEA)
	{
		tVersion = 0.0f;
		isXXTEA = false;
		char tBuffer[12];
		memset(tBuffer, 0, sizeof(tBuffer));

		char* tmp = br.ReadString(tBuffer, 8);
		if (!tmp)
			return false;

		if (strncmp("SOBJECT", tBuffer, 7) == 0)
		{
			int pVersion = tBuffer[7] - '0';
			if (pVersion == 2)
			{
				tVersion = 2.0f;
				return true;
			}
			else if (pVersion == 3)
			{
				tVersion = 2.5f;
				return true;
			}
		}
		else if (strncmp("XXTEA111", tBuffer, 8) == 0)
		{
			tVersion = 2.8f;
			isXXTEA = true;
			return true;
		}
		else if (strncmp("XXTEA222", tBuffer, 8) == 0)
		{
			tVersion = 2.9f;
			isXXTEA = true;
			return true;
		}
		else
		{

			if (strncmp("TEA1", tBuffer, 1) == 0)
			{
				//current position is 8
				//set it back to 4
				size_t offset = br.GetCurrentOffset() - 4;
				br.SetCurrentOffset(offset);
				tVersion = 1.5f;
				isXXTEA = true;
				return true;
			}

			//current position is 8
			//set it back to 0
			size_t offset = br.GetCurrentOffset() - 8;
			br.SetCurrentOffset(offset);
			tVersion = 1.0f;
			return true;
		}

		return false;
	}
}

#pragma region V1
/// <summary>
/// LoadSkin1Data
/// </summary>
namespace {

	bool LoadSkin1Texture(BinaryReader& br, SkinData1& s)
	{
		if (!s.mDiffuseMap.Load(br, "Diffuse"))
		{
			DebugA("<< !LoadSkin1Texture::mDiffuseMap()\r\n");
			return false;
		}

		if (!s.mSpecularMap.Load(br, "Specular"))
		{
			DebugA("<< !LoadSkin1Texture::mSpecularMap()\r\n");
			return false;
		}
		//if (s.mRequireFlowMap && !s.mFlowMap.Load(br, "Flow"))
		//{
		//	DebugA("<< !LoadSkin1Texture::mFlowMap()\r\n");
		//	return false;
		//}

		int mAnimationNum = br.ReadInt();
		if (mAnimationNum > 0)
		{
			DebugA("<< LoadSkin1Texture::mAnimationNum() -> %d\r\n", mAnimationNum);
			s.mAnimationMap.resize(mAnimationNum);
			for (int i = 0; i < mAnimationNum; ++i)
				if (!s.mAnimationMap[i].Load(br, std::string("Animation_").append( std::to_string(i) ).c_str() ))
					return false;
		}

		DebugA("<< LoadSkin1Texture()\r\n");

		return true;
	}

	bool LoadSkin1Buffer(BinaryReader& br, SkinData1& s)
	{
		Zlib z(br);
		if (!Zlib::Decompress(z))
		{
			DebugA("LoadSkin1Data() -> !Zlib::Decompress()");
			return false;
		}

		BinaryReader sub(&z.m_OriginalData, 0, z.m_OriginalSize);
		sub.ReadBytes(&s.mEffect, sizeof(s.mEffect));
		sub.ReadBytes(&s.mVertexNum, sizeof(s.mVertexNum));
		sub.ReadBytes(&s.mUVNum, sizeof(s.mUVNum));
		sub.ReadBytes(&s.mWeightNum, sizeof(s.mWeightNum));
		sub.ReadBytes(&s.mIndexNum, sizeof(s.mIndexNum));
		sub.ReadBytes(&s.mSize, sizeof(s.mSize));

		int mVertexNum = s.mVertexNum;
		int vtxSize = sizeof(SkinVertex1) * mVertexNum;
		auto vtx = &s.vertices;
		vtx->resize(mVertexNum);
		sub.ReadBytes( vtx->data(), vtxSize );

		auto wt = &s.weights;
		wt->resize(mVertexNum);
		sub.ReadBytes( wt->data(), vtxSize );

		int mIndexNum = s.mIndexNum;
		int idxSize = 6 * mIndexNum;
		auto idx = &s.indices;
		idx->resize(size_t(mIndexNum * 3));
		sub.ReadBytes(idx->data(), idxSize);


		auto vbuf = &s.mMotionVertex;
		auto nbuf = &s.mMotionNormal;
		//vbuf->resize( mVertexNum );
		//nbuf->resize( mVertexNum );
		for (int i = 0; i < mVertexNum; ++i)
		{
			vbuf->push_back( vtx->data()[i].mV );
			nbuf->push_back( vtx->data()[i].mN );
		}

		return LoadSkin1Texture(br, s);
	}

	bool LoadSkin1Data(BinaryReader& br, SkinData1& s, bool isXTea)
	{
		bool result = false;
		
		if(isXTea)
			result = isXTea;
		else
			result = br.ReadInt() > 0;

		if (!result)
			return true;

		return LoadSkin1Buffer(br, s);
	}
}

/// <summary>
/// LoadSkin1Base
/// </summary>
namespace {
	
	bool LoadSkin1UncompressedChunk(SkinVersion1* s, BinaryReader& br, bool isXXTEA = false, bool tRequireFlowMap = false)
	{
		//if (isXXTEA && !CheckXXTEA(br))
		//	return false;
		//
		//bool isXTea = isXXTEA;
		//bool result = false;
		//int mSkinNum = br.ReadInt();
		//if (mSkinNum > 0)
		//{
		//	s->mSkin.resize(mSkinNum);
		//	for (int i = 0; i < mSkinNum; i++)
		//	{
		//		//s->mSkin[i].mRequireFlowMap = tRequireFlowMap;
		//		result = LoadSkin1Data(br, s->mSkin[i], isXTea);
		//		isXTea = false;
		//		if (!result)
		//			break;
		//	}
		//
		//	DebugA("<< LoadSkin1UncompressedChunk::LoadSkin2Data() -> %d\r\n", result);
		//}
		return false;
	}


	/// <summary>
	/// 
	/// </summary>
	/// <param name="s"></param>
	/// <param name="br"></param>
	/// <param name="isXXTEA"> = for XXTEA111 or XXTEA222</param>
	/// <param name="tRequireFlowMap"> = for XXTEA222</param>
	/// <returns></returns>
	bool LoadSkin1CompressedChunk(SkinVersion1* s, BinaryReader& br, bool isXXTEA = false, bool tRequireFlowMap = false)
	{
		if (isXXTEA && !CheckXXTEA(br))
			return false;

		DebugA("<< br.offset = %d\r\n", (int)br.GetCurrentOffset());

		bool isXTea = isXXTEA;
		bool result = false;
		int mSkinNum = br.ReadInt();

		if (mSkinNum > 0)
		{
			s->mSkin.resize(mSkinNum);
			for (int i = 0; i < mSkinNum; i++)
			{
				//s->mSkin[i].mRequireFlowMap = tRequireFlowMap;
				result = LoadSkin1Data(br, s->mSkin[i], isXTea);
				isXTea = false;
				if (!result)
					break;
			}
		}
		DebugA("<< LoadSkin1CompressedChunk::LoadSkin2Data() -> %d\r\n", result);

		return result;
	}
}

#pragma endregion V1

#pragma region V2
/// <summary>
/// LoadSkin2Data
/// </summary>
namespace {

	bool LoadSkin2Texture(BinaryReader& br, SkinData2& s)
	{
		if (!s.mDiffuseMap.Load(br, "Diffuse"))
		{
			DebugA("<< !LoadSkin2Texture::mDiffuseMap()\r\n");
			return false;
		}

		if (!s.mNormalMap.Load(br, "Normal"))
		{
			DebugA("<< !LoadSkin2Texture::mNormalMap()\r\n");
			return false;
		}

		if (!s.mSpecularMap.Load(br, "Specular"))
		{
			DebugA("<< !LoadSkin2Texture::mSpecularMap()\r\n");
			return false;
		}
		if (s.mRequireFlowMap && !s.mFlowMap.Load(br, "Flow"))
		{
			DebugA("<< !LoadSkin2Texture::mFlowMap()\r\n");
			return false;
		}

		int mAnimationNum = br.ReadInt();
		if (mAnimationNum > 0)
		{
			DebugA("<< LoadSkin2Texture::mAnimationNum() -> %d\r\n", mAnimationNum);
			s.mAnimationMap.resize(mAnimationNum);
			for (int i = 0; i < mAnimationNum; ++i)
				if (!s.mAnimationMap[i].Load(br, std::string("Animation_").append( std::to_string(i) ).c_str() ))
					return false;
		}

		DebugA("<< LoadSkin2Texture()\r\n");

		return true;
	}

	bool LoadSkin2Buffer(BinaryReader& br, SkinData2& s)
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
			vtx->resize( mVertexNum );
			br.ReadBytes( vtx->data(), vtxSize );


			int mIndexNum = l->mIndexNum = br.ReadInt();
			int idxSize = 6 * mIndexNum;
			auto idx = &l->indices;
			idx->resize( size_t( mIndexNum * 3 ) );
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

	bool LoadSkin2Data(BinaryReader& br, SkinData2& s)
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

		s.mCheckValidState = LoadSkin2Buffer(br, s);
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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="s"></param>
	/// <param name="br"></param>
	/// <param name="isXXTEA"> = for XXTEA</param>
	/// <param name="tRequireFlowMap"> = for XXTEA222</param>
	/// <returns></returns>
	bool LoadSkin2UncompressedChunk(SkinVersion2* s, BinaryReader& br, bool isXXTEA = false, bool tRequireFlowMap = false)
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
				s->mSkin[i].mRequireFlowMap = tRequireFlowMap;
				result = LoadSkin2Data(br, s->mSkin[i]);
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
	/// <param name="tRequireFlowMap"> = for XXTEA222</param>
	/// <returns></returns>
	bool LoadSkin2CompressedChunk(SkinVersion2* s, BinaryReader& br, bool isXXTEA = false, bool tRequireFlowMap = false)
	{
		if (isXXTEA && !CheckXXTEA(br))
			return false;

		Zlib z( br );
		if (!Zlib::Decompress(z))
		{
			DebugA("<< !LoadSkin2CompressedChunk::Decompress()\r\n");
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
				s->mSkin[i].mRequireFlowMap = tRequireFlowMap;
				result = LoadSkin2Data(sub, s->mSkin[i]);
				if (!result)
					break;
			}
		}
		DebugA("<< LoadSkin2CompressedChunk::LoadSkin2Data() -> %d\r\n", result);

		return result;
	}


}

#pragma endregion V2

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
		
		float tVersion;
		bool tValid = false;
		char tCompressed = '0';
		bool tRequireFlowMap = false;
		bool isXXTEA = false;

		BinaryReader br( &data, 0, data.size() );
		if (!LoadSkinHeader(br, tVersion, isXXTEA))
		{
			DebugA("%s << !LoadSkin2Header()\r\n", tFileName);
			return result;
		}

		if ( tVersion == 1.0f )//TwelveSky1 | TwelveSk2 Aeria|MaynGames|AsiaSoft
		{
			tValid = true;
			tCompressed = '1';
		}
		else if ( tVersion == 1.5f )//TwelveSky1 GXCW  with TEA1 | TwelveSky2 GXCW SOBJECT v2.0 with TEA1
		{
			tValid = true;
			tCompressed = '1';
		}
		else if ( tVersion == 2.0f )//Troy vs Sparta | Waren Story
		{
			tValid = true;
		}
		else if( tVersion == 2.5f )//TwelveSky2.5 - TwelveSky2GXCW
		{
			if (!LoadSkin2Extra(br, tValid, tCompressed))
			{
				DebugA("%s << !LoadSkin2Extra()\r\n", tFileName);
			}
		}
		else if( tVersion == 2.8f )//TwelveSky2GXCW - XXTEA111
		{
			if (!LoadSkin2Extra(br, tValid, tCompressed))
			{
				DebugA("%s << !LoadSkin2Extra()\r\n", tFileName);
			}
		}
		else if( tVersion == 2.9f )//TwelveSky2GXCW - XXTEA222 (Diffuse+Normal+Specular + Flow Mapping Texture)
		{
			if (!LoadSkin2Extra(br, tValid, tCompressed))
			{
				DebugA("%s << !LoadSkin2Extra()\r\n", tFileName);
			}
			tRequireFlowMap = true;
		}

		printf("processing %s >> valid: %d, version: %.1f, isXXTEA: %d\r\n", tFileName, tValid, tVersion, isXXTEA);
		if (tValid)
		{
			if ( tVersion >= 1.0f && tVersion <= 1.5f )
			{
				skin->v1 = new SkinVersion1();
				if (tCompressed != '0') {
					result = LoadSkin1CompressedChunk(skin->v1, br, isXXTEA, tRequireFlowMap);
				}
				else {
					result = LoadSkin1UncompressedChunk(skin->v1, br, isXXTEA, tRequireFlowMap);
				}
				skin->Create1(result);
			}
			else if( tVersion >= 2.0f )
			{
				skin->v2 = new SkinVersion2();
				if (tCompressed != '0') {
					result = LoadSkin2CompressedChunk(skin->v2, br, isXXTEA, tRequireFlowMap);
				}
				else {
					result = LoadSkin2UncompressedChunk(skin->v2, br, isXXTEA, tRequireFlowMap);
				}
				skin->Create2(result);
			}
		}
		printf("result %s >> %s\r\n", tFileName, result ? "succeeded" : "failed" );

		return result;
	}

}