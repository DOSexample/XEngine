#pragma once


#include <d3dx9.h>

class XTransform
{
public:
	XTransform();

	void SetPosition(D3DXVECTOR3 v);
	void SetRotation(D3DXVECTOR3 v);
	void SetRotation(D3DXQUATERNION v);
	void SetScale(D3DXVECTOR3 v);

	D3DXVECTOR3 GetPosition();
	D3DXQUATERNION GetRotation();
	D3DXVECTOR3 GetScale();

	virtual void Update();
	D3DXMATRIX GetWorldMatrix();

private:
	D3DXVECTOR3 m_localPosition;
	D3DXVECTOR3 m_localRotation;
	D3DXVECTOR3 m_localScale;
	D3DXMATRIX m_Matrix;

};