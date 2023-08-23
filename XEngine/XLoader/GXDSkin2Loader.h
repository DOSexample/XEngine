#pragma once

class XSkinMesh;

namespace XLoader {


	class GXDSkin2Loader
	{
	public:
		GXDSkin2Loader();

		bool Load( const char* tFileName, XSkinMesh* mSkin );

	};

}