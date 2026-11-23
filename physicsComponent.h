#pragma once
#include "mathUtils.h"
#include "transformComponent.h"
#include "component.h"



class PhysicsComponent : public Component
{
public:

	void SetVelocity(const Vector3& velocity) { m_Velocity = velocity; }
	void AddVelocity(Vector3 force) { m_Velocity += force; }
	Vector3 GetVelocity() const { return m_Velocity; }

	void SetMass(float mass) { m_Mass = mass; }
	float GetMass() const { return m_Mass; }

	void SetGravity(float gravity) { m_Gravity = gravity; }
	void SetUseGravity(bool enabled) { m_UseGravity = enabled; }

	
	void AddForce(Vector3 force) { m_Acceleration += force / m_Mass; } // a = F / m

	void Init() override;
	void Update(float deltaTime) override;


private:

	Vector3 m_Velocity;       //速度
	Vector3 m_Acceleration;   //加速度
	float   m_Mass = 1.0f;    //質量
	float   m_Gravity = 39.8f; //重力加速度
	float   m_Damping = 0.9f; //減衰係数

	//使用フラグ
	bool m_UseGravity = false;

	TransformComponent* m_Transform = nullptr;
};
