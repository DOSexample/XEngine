#pragma once

class XAnimationMotion;
namespace XLoader {

	class GXDMotionLoader
	{
	public:
		GXDMotionLoader();
		bool Load(const char* tFileName, XAnimationMotion* anim);
	};

}