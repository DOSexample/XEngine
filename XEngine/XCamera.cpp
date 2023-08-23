#include "XCamera.h"

#include "XDevice.h"

D3DVIEWPORT9 mViewport;
D3DXMATRIX mViewportMatrix;
D3DXMATRIX mPerspectiveMatrix;
D3DXVECTOR3 mCameraEye;
D3DXVECTOR3 mCameraLook;
D3DXVECTOR3 mCameraUp;
D3DXVECTOR3 mCameraForward;
D3DXMATRIX mViewMatrix;
float mFovY;

XCamera::XCamera()
{
}

void XCamera::Initialize(int tScreenXSize, int tScreenYSize, float tFovY, float tNearPlane, float tFarPlane)
{
    auto device = &XDevice::Instance();

    float sX = static_cast<float>( tScreenXSize );
    float sY = static_cast<float>( tScreenYSize );
    
    mCameraEye = D3DXVECTOR3( 0.0f, 5.0f, -38.0f );
    mCameraLook = D3DXVECTOR3( 0.0f, 10.0f, 0.0f );
    mCameraUp = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

    mFovY = tFovY;
    mViewport.X = 0;
    mViewport.Y = 0;
    mViewport.Width = tScreenXSize;
    mViewport.Height = tScreenYSize;
    mViewport.MinZ = 0.0f;
    mViewport.MaxZ = 1.0f;
    device->SetViewport(&mViewport);

    mViewportMatrix._11 = sX * 0.5f;
    mViewportMatrix._12 = 0.0f;
    mViewportMatrix._13 = 0.0f;
    mViewportMatrix._14 = 0.0f;
    mViewportMatrix._21 = 0.0f;
    mViewportMatrix._22 = -sY * 0.5f;
    mViewportMatrix._23 = 0.0f;
    mViewportMatrix._24 = 0.0f;
    mViewportMatrix._31 = 0.0f;
    mViewportMatrix._32 = 0.0f;
    mViewportMatrix._33 = 1.0f;
    mViewportMatrix._34 = 0.0f;
    mViewportMatrix._41 = sX * 0.5f;
    mViewportMatrix._42 = sY * 0.5f;
    mViewportMatrix._43 = 0.0f;
    mViewportMatrix._44 = 1.0f;
    float Aspect = sX / sY;
    D3DXMatrixPerspectiveFovLH( &mPerspectiveMatrix, mFovY * D3DXToRadian(1.0f), Aspect, tNearPlane, tFarPlane );
}

void XCamera::Update()
{
    auto device = &XDevice::Instance();

    mCameraForward = mCameraLook - mCameraEye;
    D3DXVec3Normalize( &mCameraForward, &mCameraForward );
    D3DXMatrixLookAtLH( &mViewMatrix, &mCameraEye, &mCameraLook, &mCameraUp );

    device->SetTransform(D3DTS_PROJECTION, &mPerspectiveMatrix);
    device->SetTransform(D3DTS_VIEW, &mViewMatrix);
}

void XCamera::SetEye(float x, float y, float z)
{
    mCameraEye = D3DXVECTOR3(x, y, z);
}

void XCamera::SetLook(float x, float y, float z)
{
    mCameraLook = D3DXVECTOR3(x, y, z);
}

D3DXMATRIX* XCamera::GetViewMatrix()
{
    return &mViewMatrix;
}

D3DXMATRIX* XCamera::GetPerspectiveMatrix()
{
    return &mPerspectiveMatrix;
}
