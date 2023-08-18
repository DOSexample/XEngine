#include <X/Engine/Component/Transform.h>

#include <X/Graphics/Graphic.h>

#include <X/Engine/Timer.h>
#include <X/Core/Debug.h>

namespace X {

    Transform::Transform() : Component(),
        m_localPosition(0.0f, 0.0f, 0.0f),
        m_localRotation(0.0f, 0.0f, 0.0f),
        m_localScale(1.0f, 1.0f, 1.0f),
        m_parent(nullptr)
	{
        m_Type = XOBJ_TRANSFORM;
	}

    Transform::~Transform() {
    }

    D3DXVECTOR3 Transform::GetPosition() const {
        if (m_parent) {
            D3DXMATRIX worldMatrix = GetWorldMatrix();
            return D3DXVECTOR3(worldMatrix._41, worldMatrix._42, worldMatrix._43); // Extract translation from matrix
        }
        return m_localPosition;
    }

    D3DXVECTOR3 Transform::GetRotation() const {
        D3DXMATRIX worldMatrix = GetWorldMatrix();

        // Convert the rotation matrix part of the worldMatrix to a quaternion
        D3DXQUATERNION quaternion;
        D3DXQuaternionRotationMatrix(&quaternion, &worldMatrix);

        // Convert the quaternion to Euler angles
        D3DXVECTOR3 eulerRotation;
        D3DXVec3BaryCentric(&eulerRotation, &m_localRotation, &m_localRotation, &m_localRotation, 2.0f * asinf(quaternion.y), atan2f(quaternion.z, quaternion.w));

        return eulerRotation;
    }

    D3DXVECTOR3 Transform::GetScale() const {
        D3DXMATRIX worldMatrix = GetWorldMatrix();

        D3DXVECTOR3 scale;
        D3DXVECTOR3 x(worldMatrix._11, worldMatrix._12, worldMatrix._13);
        D3DXVECTOR3 y(worldMatrix._21, worldMatrix._22, worldMatrix._23);
        D3DXVECTOR3 z(worldMatrix._31, worldMatrix._32, worldMatrix._33);
        scale.x = D3DXVec3Length(&x); // Length of the first row
        scale.y = D3DXVec3Length(&y); // Length of the second row
        scale.z = D3DXVec3Length(&z); // Length of the third row

        return scale;
    }

    D3DXMATRIX Transform::GetWorldMatrix() const {
        if (m_parent) {
            return GetLocalMatrix() * m_parent->GetWorldMatrix();
        }
        return GetLocalMatrix();
    }

    D3DXMATRIX Transform::GetLocalMatrix() const {

        //D3DXMATRIX matTranslation, matRX, matRY, matRZ, matScale;;
        //D3DXMatrixTranslation(&matTranslation, m_localPosition.x, m_localPosition.y, m_localPosition.z);
        //D3DXMatrixRotationX(&matRX, m_localRotation.x * D3DXToRadian(1.0f));
        //D3DXMatrixRotationY(&matRY, m_localRotation.y * D3DXToRadian(1.0f));
        //D3DXMatrixRotationZ(&matRZ, m_localRotation.z * D3DXToRadian(1.0f));
        //D3DXMatrixScaling(&matScale, m_localScale.x, m_localScale.y, m_localScale.z);
        //
        //return matScale * ( matRZ * matRY * matRX ) * matTranslation;

        D3DXMATRIX matTranslation, matRotation, matScale;
        D3DXMatrixTranslation(&matTranslation, m_localPosition.x, m_localPosition.y, m_localPosition.z);
        D3DXMatrixRotationYawPitchRoll(&matRotation, m_localRotation.x, m_localRotation.y, m_localRotation.z);
        //D3DXMatrixRotationYawPitchRoll(&matRotation, m_localRotation.x * D3DXToRadian(1.0f), m_localRotation.y * D3DXToRadian(1.0f), m_localRotation.z * D3DXToRadian(1.0f));
        D3DXMatrixScaling(&matScale, m_localScale.x, m_localScale.y, m_localScale.z);

        return matScale * matRotation * matTranslation;
    }
}