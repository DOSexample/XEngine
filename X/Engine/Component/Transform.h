#pragma once

#include <X/Engine/Component.h>

namespace X {

	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

        static EXOBJECT_TYPE ConstType()
        {
            return XOBJ_TRANSFORM;
        }

        D3DXVECTOR3 m_localPosition;
        D3DXVECTOR3 m_localRotation;
        D3DXVECTOR3 m_localScale;

        Transform* m_parent;

        void SetParent(Transform* parent) {
            m_parent = parent;
        }

        void SetPosition(const D3DXVECTOR3& position) {
            m_localPosition = position;
        }

        void SetRotation(const D3DXVECTOR3& rotation) {
            m_localRotation = rotation;
        }

        void SetScale(const D3DXVECTOR3& scale) {
            m_localScale = scale;
        }

        D3DXVECTOR3 GetPosition() const;
        D3DXVECTOR3 GetRotation() const;
        D3DXVECTOR3 GetScale() const;
        D3DXMATRIX GetWorldMatrix() const;

    private:
        D3DXMATRIX GetLocalMatrix() const;
	};

}