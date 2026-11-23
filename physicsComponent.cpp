#include "physicsComponent.h"
#include "gameObject.h"


void PhysicsComponent::Init()
{
	m_Transform = GetOwner()->GetComponent<TransformComponent>();
}


void PhysicsComponent::Update(float deltaTime)
{
	if (!m_Transform) return;


	//重力
	if (m_UseGravity)
	{
		m_Velocity.y -= m_Gravity * deltaTime;
	}

	//速度の更新
	m_Velocity += m_Acceleration * deltaTime;

	//減衰
	m_Velocity.x *= m_Damping;
	m_Velocity.z *= m_Damping;

	//位置の更新 
	m_Transform->AddPosition(m_Velocity * deltaTime);


	//加速度のリセット
	m_Acceleration = { 0, 0, 0 };
}
