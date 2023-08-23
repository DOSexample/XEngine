#include "XTransform.h"

XTransform::XTransform()
    :
    m_localPosition(D3DXVECTOR3(0, 0, 0)),
    m_localRotation(D3DXVECTOR3(0, 0, 0)),
    m_localScale(D3DXVECTOR3(1, 1, 1))
{
    D3DXMatrixIdentity(&m_Matrix);
}

void XTransform::SetPosition(D3DXVECTOR3 v)
{
    m_localPosition = v;
}

void XTransform::SetRotation(D3DXVECTOR3 v)
{
    m_localRotation = v;
}

void XTransform::SetRotation(D3DXQUATERNION v)
{
    m_localRotation.x = v.x;
    m_localRotation.y = v.y;
    m_localRotation.z = v.z;
}

void XTransform::SetScale(D3DXVECTOR3 v)
{
    m_localScale = v;
}

D3DXVECTOR3 XTransform::GetPosition()
{
    return m_localPosition;
}

D3DXQUATERNION XTransform::GetRotation()
{
    return D3DXQUATERNION(m_localRotation.x, m_localRotation.y, m_localRotation.z, 1.0f);
}

D3DXVECTOR3 XTransform::GetScale()
{
    return m_localScale;
}

void XTransform::Update()
{
    D3DXMATRIX matT, matRX, matRY, matRZ, matS;
    D3DXMatrixTranslation(&matT, m_localPosition.x, m_localPosition.y, m_localPosition.z);
    D3DXMatrixRotationX(&matRX, m_localRotation.x * D3DXToRadian(1.0f));
    D3DXMatrixRotationY(&matRY, m_localRotation.y * D3DXToRadian(1.0f));
    D3DXMatrixRotationZ(&matRZ, m_localRotation.z * D3DXToRadian(1.0f));
    //D3DXMatrixRotationYawPitchRoll( &matRZ, m_localRotation.y * D3DXToRadian(1.0f), m_localRotation.x * D3DXToRadian(1.0f), m_localRotation.z * D3DXToRadian(1.0f));
    D3DXMatrixScaling(&matS, m_localScale.x, m_localScale.y, m_localScale.z);
    m_Matrix = matS * (matRZ * matRY * matRX) * matT;
    //xTRSMatrix = matS * matRZ * matT;
}

D3DXMATRIX XTransform::GetWorldMatrix()
{
    return m_Matrix;
}