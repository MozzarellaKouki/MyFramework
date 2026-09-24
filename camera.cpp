#include "Camera.h"
#include "keyboard.h"
#include "renderer.h"
#include "transformComponent.h"


void Camera::Init()
{
    m_Transform = AddComponent<TransformComponent>();
    m_Transform->SetPosition({ 0,1,-5 });
    m_Target = { 0.0f, 2.0f,  0.0f };
    m_Up = { 0.0f, 1.0f,  0.0f };

    m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);

    m_Speed = 0.1f;

}


void Camera::Update()
{
    UpdateFollowTarget();

    Vector3 eyePos = m_Transform->GetPosition();
    XMVECTOR eye = XMVectorSet(  eyePos.x, eyePos.y, eyePos.z, 1.0f);
    XMVECTOR target = XMVectorSet(m_Target.x, m_Target.y, m_Target.z, 1.0f);
    XMVECTOR up = XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0.0f);

    m_ViewMatrix = XMMatrixLookAtLH(eye, target, up);  
    m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
    m_BillboardMatrix = XMMatrixInverse(nullptr, m_ViewMatrix);
    m_BillboardMatrix.r[3] = XMVectorSet(0, 0, 0, 1);

}


void Camera::Apply()
{
    Renderer::SetViewMatrix(m_ViewMatrix);
    Renderer::SetProjectionMatrix3D();
}




void Camera::UpdateFollowTarget()
{
    if (m_FollowTarget)
    {
        auto transform = m_FollowTarget->GetComponent<TransformComponent>();

        if (transform)
        {
            Vector3 pos = transform->GetPosition();
            Vector3 rot = transform->GetRotation();

            float yaw = rot.y;

            Vector3 forward;

            forward.x = sinf(yaw);
            forward.y = 0.0f;
            forward.z = cosf(yaw);

            m_Transform->SetPosition(
                {
                    pos.x - forward.x * 10.0f,
                    pos.y + 2.0f,
                    pos.z - forward.z * 10.0f
                });



            m_Target = { pos.x,  pos.y + 2.0f, pos.z };
        }
    }

}


Vector3 Camera::GetForward() const
{
    Vector3 eyePos = m_Transform->GetPosition();

    Vector3 forward;
    forward.x = m_Target.x - eyePos.x;
    forward.y = m_Target.y - eyePos.y;
    forward.z = m_Target.z - eyePos.z;

    float length = sqrtf(
        forward.x * forward.x +
        forward.y * forward.y +
        forward.z * forward.z
    );

    if (length > 0.0f)
    {
        forward.x /= length;
        forward.y /= length;
        forward.z /= length;
    }

    return forward;
}
