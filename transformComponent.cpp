#include "main.h"
#include "transformComponent.h"






XMMATRIX TransformComponent::GetWorldMatrix() const
{
    return
        XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)
        *
        XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z)
        *
        XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
}

