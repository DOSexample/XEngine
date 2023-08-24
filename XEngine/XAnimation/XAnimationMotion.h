#pragma once

#include <map>
#include <vector>

struct D3DXVECTOR3;
struct D3DXMATRIX;

class XAnimationMotion
{
public:

	XAnimationMotion();
	~XAnimationMotion();

	void Create(bool isSuccess);

	int mCurAnimFrame;
	int mFrameNum;
	int mBoneNum;
	std::map<int, std::vector<D3DXMATRIX>> mMatrixKey;
};
