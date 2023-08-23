#pragma once

#include <vector>

#include <d3dx9.h>

#include "../XSystem/IO/BinaryReader.h"

using namespace XSystem::IO;

namespace XLoader {

	class GXDTextureLoader
	{
	public:
		GXDTextureLoader();
		~GXDTextureLoader();

		bool mCheckValidState;
		unsigned long mFileDataSize;
		unsigned char* mFileData;
		D3DXIMAGE_INFO mTextureInfo;
		int mProcessModeCase;
		int mAlphaModeCase;
		int mOrgAlphaModeCase;
		IDirect3DTexture9* mTexture;


		void Free();
		bool Load(BinaryReader& br);
	};

}