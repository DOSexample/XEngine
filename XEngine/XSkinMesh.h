#pragma once

#include "XDevice.h"

#include "XLoader/GXDSkin2Loader.h"
#include "XLoader/GXDTextureLoader.h"

#include "XTexture.h"
#include "XTransform.h"

#include <vector>

#pragma region V1
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

typedef struct
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
} LoDSkin;

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
	XTexture mAlbedoMap;
	bool mRequireAlbedoMap;
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

	SkinVersion2* v2;
	/// <summary>
	/// Create and Upload VertexBuffer / IndexBuffer
	/// </summary>
	void Create2(bool isSuccess);

	void Draw(XAnimationMotion* anim = 0);

};

