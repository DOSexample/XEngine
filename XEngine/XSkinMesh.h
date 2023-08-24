#pragma once

#include "XDevice.h"

#include "XLoader/GXDSkinLoader.h"
#include "XLoader/GXDTextureLoader.h"

#include "XTexture.h"
#include "XTransform.h"

#include <vector>

#pragma region V1
struct SkinVertex1
{
	float mV[3];
	float mN[3];
	float mT[2];
};

struct SkinSize1
{
	float mBoxMin[3];
	float mBoxMax[3];
	float mCenter[3];
	float mRadius;
};

struct SkinEffect1
{
	BOOL mCheckTwoSide;
	BOOL mCheckRadiation;
	float mRadiationSpeed;
	float mRadiationLowLimit[4];
	float mRadiationHighLimit[4];
	BOOL mCheckLightBright;
	BOOL mCheckCameraSpecularEffect;
	int mCameraSpecularEffectSort;
	float mCameraSpecularEffectMaterialValue[4];
	float mCameraSpecularEffectMaterialPower;
	float mCameraSpecularEffectLightAddValue;
	BOOL mCheckTextureAnimation;
	float mTextureAnimationSpeed;
	BOOL mCheckUVScroll1;
	int mUVScrollSort1;
	float mUVScrollSpeed1;
	BOOL mCheckBillboard;
	int mBillboardSort;
	BOOL mCheckUVScroll2;
	int mUVScrollSort2;
	float mUVScrollSpeed2;
};

struct SkinWeight
{
	int mBoneIndex[4];
	float mBlendValue[4];
};

struct SkinData1
{
	SkinVertex1 mVertexBufferForBillboard[4];
	SkinEffect1 mEffect;

	int mVertexNum;
	int mUVNum;
	int mWeightNum;
	int mIndexNum;

	std::vector<D3DXVECTOR3> mMotionVertex;
	std::vector<D3DXVECTOR3> mMotionNormal;
	//std::vector<D3DXVECTOR2> mUV;
	//std::vector<D3DXVECTOR3> mWeight;
	//std::vector<WORD> mTris;

	SkinSize1 mSize;

	IDirect3DVertexBuffer9* mVertexBuffer;
	std::vector<SkinVertex1> vertices;

	std::vector<SkinWeight> weights;

	IDirect3DIndexBuffer9* mIndexBuffer;
	std::vector<WORD> indices;

	XTexture mDiffuseMap;
	XTexture mSpecularMap;
	std::vector<XTexture> mAnimationMap;
};

struct SkinVersion1
{
	std::vector<SkinData1> mSkin;
};
#pragma endregion V1

#pragma region V2
struct SkinEffect2
{
	int mAnimationMapInfo[2];
	int mRadiationInfo[10];
	int mBillboardInfo[2];
};

struct SkinSize2
{
	float mBoxSize[3];
};

struct SkinVertex2
{
	float mV[3];
	float mW[4];
	DWORD mB;
	float mN1[3];
	float mN2[3];
	float mN3[3];
	float mT[2];
};

struct SkinShadow2
{
	float mV[3];
	float mW[4];
	DWORD mB;
};

struct LoDSkin
{
	int mVertexNum;
	int mIndexNum;

	IDirect3DVertexBuffer9* mVertexBuffer;
	std::vector<SkinVertex2> vertices;

	IDirect3DIndexBuffer9* mIndexBuffer;
	std::vector<WORD> indices;
	std::vector<SkinShadow2> mShadowVertexBuffer;
	std::vector<WORD> mShadowIndexBuffer;
	std::vector<WORD> mShadowEdgeBuffer;
};

struct SkinData2
{
	BOOL mCheckValidState;
	SkinEffect2 mEffect;
	SkinSize2 mSize;
	SkinVertex2 mVertexBufferForBillboard[4];
	SkinVertex2 mVertexBillboardForUse[4];
	std::vector<LoDSkin> mLOD;
	XTexture mDiffuseMap;
	XTexture mNormalMap;
	XTexture mSpecularMap;
	XTexture mFlowMap;
	bool mRequireFlowMap;
	std::vector<XTexture> mAnimationMap;
};

struct SkinVersion2
{
	std::vector<SkinData2> mSkin;
};
#pragma endregion V2

enum GXD_SKIN_VERSION
{
	V0,
	V1,
	V2,
	V3,
	V4,
	V5
};

class XAnimationMotion;
class XTransform;
class XSkinMesh : public XTransform
{
public:

	XSkinMesh();
	~XSkinMesh();
	void Release();

	GXD_SKIN_VERSION mSkinVersion;

	SkinVersion1* v1;
	/// <summary>
	/// Create and Upload VertexBuffer / IndexBuffer
	/// </summary>
	void Create1(bool isSuccess);


	SkinVersion2* v2;
	/// <summary>
	/// Create and Upload VertexBuffer / IndexBuffer
	/// </summary>
	void Create2(bool isSuccess);

	void Draw(XAnimationMotion* anim = 0);

private:
	void ReleaseV1();
	void ReleaseV2();
};

