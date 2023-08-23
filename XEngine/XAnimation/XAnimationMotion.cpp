#include "XAnimationMotion.h"

#include "../XDevice.h"

XAnimationMotion::XAnimationMotion()
	:
	mCurAnimFrame(0),
	mFrameNum(0),
	mBoneNum(0),
	mMatrixKey{}
{
}

XAnimationMotion::~XAnimationMotion()
{
	if (!mMatrixKey.empty())
		for (int i = 0; i < (int)mMatrixKey.size(); ++i)
			for(int j = 0; j < (int)mMatrixKey[i].size(); ++j)
				if(mMatrixKey[i][j])
					delete mMatrixKey[i][j];
	mMatrixKey.clear();
}

void XAnimationMotion::Create(bool isSuccess)
{
	if (!isSuccess)
	{
		mFrameNum = 0;
		mBoneNum = 0;
		mMatrixKey.clear();
		return;
	}
}
