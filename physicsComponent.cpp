#include "physicsComponent.h"
#include "gameObject.h"



void PhysicsComponent::Update(float deltaTime)
{
    auto transform = GetOwner()->GetComponent<TransformComponent>();
    if (!transform) return;

    m_Velocity += m_Acceleration * deltaTime;
    m_Velocity *= m_Damping;
    transform->AddPosition(m_Velocity * deltaTime);

    m_Acceleration = { 0, 0, 0 };
}
