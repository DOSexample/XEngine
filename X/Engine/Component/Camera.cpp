#include <X/Engine/Component/Camera.h>

#include <X/Graphics/Graphic.h>

#include <X/Engine/Timer.h>

#include <X/Core/Debug.h>

namespace X {

    Camera::Camera() : Component(),
        m_position(0.0f, 0.0f, -5.0f),
        m_target(0.0f, 0.0f, 0.0f),
        m_up(0.0f, 1.0f, 0.0f),
        m_fov(D3DX_PI / 4),
        m_aspectRatio(800.0f / 600.0f),  // Assuming 800x600 screen size
        m_nearClip(0.1f),
        m_farClip(1000.0f)
    {
        m_Type = XOBJ_CAMERA;
    }

    Camera::~Camera() {
    }

    void Camera::Update()
    {
        DebugA("%s\r\n", __FUNCTION__);
        auto device = gfx->GetDevice();

        D3DXMATRIX viewMatrix, projMatrix;

        D3DXMatrixLookAtLH(&viewMatrix, &m_position, &m_target, &m_up);
        D3DXMatrixPerspectiveFovLH(&projMatrix, m_fov, m_aspectRatio, m_nearClip, m_farClip);

        device->SetTransform(D3DTS_VIEW, &viewMatrix);
        device->SetTransform(D3DTS_PROJECTION, &projMatrix);
    }

    void Camera::SetPosition(const D3DXVECTOR3& pos) {
        m_position = pos;
    }

    void Camera::LookAt(const D3DXVECTOR3& target) {
        m_target = target;
    }

    void Camera::SetUp(const D3DXVECTOR3& up) {
        m_up = up;
    }
}