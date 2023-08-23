#pragma once

#include <d3dx9.h>
#include "XRenderPass.h"

#include "XSystem/IO/BinaryReader.h"

class XTexture
{
public:

	bool mCheckValidState;
	unsigned long mFileDataSize;
	unsigned char* mFileData;
	D3DXIMAGE_INFO mTextureInfo;


	int mProcessModeCase;

	//Alpha Type
	//1 = AplhaTest
	//2 = AlphaBlend
	int mAlphaModeCase;

	int mOrgAlphaModeCase;
	IDirect3DTexture9* mTexture;

	XTexture();
	~XTexture();

	void Free();
	bool Load(XSystem::IO::BinaryReader& br);
};

