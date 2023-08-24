#include "XTexture.h"

#include "XSystem/IO/BinaryReader.h"
#include "XSystem/IO/Zlib.h"

#include "XDevice.h"

#include <fstream>

using namespace XSystem::IO;

static int s_TextureID = 0;
namespace {
	BOOL IsDXT(D3DFORMAT format)
	{
		return format == D3DFMT_DXT1 ||
			//format == D3DFMT_DXT2 || 
			format == D3DFMT_DXT3 || format == D3DFMT_DXT5;
	}
}

XTexture::XTexture()
	: mCheckValidState(0)
	, mFileData(nullptr)
	, mFileDataSize(0)
	, mTexture(nullptr)
	, mTextureInfo()
	, mAlphaModeCase(0)
	, mProcessModeCase(0)
	, mOrgAlphaModeCase(0)
{
}

XTexture::~XTexture()
{
	Free();
}

void XTexture::Free()
{
	if (mFileData)
	{
		delete[] mFileData;
		mFileData = nullptr;
	}

	if (mTexture)
	{
		mTexture->Release();
		mTexture = nullptr;
	}
}

bool XTexture::Load(BinaryReader& br, const char* name)
{
	mFileDataSize = br.ReadInt();
	if (mFileDataSize < 1) return true;

	Zlib z(br);
	if (!Zlib::Decompress(z))
		return false;

	mFileData = new unsigned char[mFileDataSize];

	memcpy(mFileData, z.m_OriginalData.data(), mFileDataSize);
	memcpy(&mProcessModeCase, z.m_OriginalData.data() + mFileDataSize, 4);
	memcpy(&mAlphaModeCase, z.m_OriginalData.data() + mFileDataSize + 4, 4);

	HRESULT hr = D3DXGetImageInfoFromFileInMemory(mFileData, mFileDataSize, &mTextureInfo);
	if (FAILED(hr)) return false;

	UINT width = mTextureInfo.Width;
	UINT height = mTextureInfo.Height;
	D3DFORMAT format = mTextureInfo.Format;

	printf("width: %d, height: %d, Alpha: %d \r\n", width, height, mAlphaModeCase );

	//for export texture to file
	//char texName[256];
	//sprintf(texName, "Tex_%s_%d.dds", name, s_TextureID++);
	//std::ofstream fs( texName, std::ios::out | std::ios::binary | std::ios::app);
	//fs.write((const char*)mFileData, mFileDataSize);
	//fs.close();

	if ( IsDXT(format) && XDevice::Instance().CreateTexture(mFileData, mFileDataSize, width, height, 0, 0, format, D3DPOOL_MANAGED, 1, 1, 0, 0, 0, &mTexture) )
		mCheckValidState = true;

	delete[] mFileData;
	mFileData = nullptr;

	return mCheckValidState;
}
