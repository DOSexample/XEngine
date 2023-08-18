#pragma once

#include <X/Engine/Component.h>

#include <d3dx9.h>

namespace X {

	class Camera : public Component
	{
	public:
		Camera();
		~Camera();

		void Update();

		void SetPosition(const D3DXVECTOR3& pos);
		void LookAt(const D3DXVECTOR3& target);
		void SetUp(const D3DXVECTOR3& up);

		D3DXVECTOR3 m_position;
		D3DXVECTOR3 m_target;
		D3DXVECTOR3 m_up;

		float m_fov; // Field of View
		float m_aspectRatio;
		float m_nearClip;
		float m_farClip;

		static EXOBJECT_TYPE ConstType()
		{
			return XOBJ_CAMERA;
		}
	};

}