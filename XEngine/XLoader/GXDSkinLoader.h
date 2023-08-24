#pragma once

class XSkinMesh;

namespace XLoader {


	class GXDSkinLoader
	{
	public:
		GXDSkinLoader();

		bool Load( const char* tFileName, XSkinMesh* mSkin );

	};

}