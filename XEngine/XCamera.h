#pragma once

struct D3DXMATRIX;
class XCamera
{
public:
	XCamera();

	void Initialize(int tScreenXSize, int tScreenYSize, float tFovY, float tNearPlane, float tFarPlane);

	void Update();

	void SetEye(float x, float y, float z);
	void SetLook(float x, float y, float z);

	D3DXMATRIX* GetViewMatrix();
	D3DXMATRIX* GetPerspectiveMatrix();
};

