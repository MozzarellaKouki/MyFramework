#pragma once
#include "mathUtils.h"
#include "transformComponent.h"
#include "component.h"



class PhysicsComponent : public Component
{
private:

    Vector3 m_Velocity;       //‘¬“x
    Vector3 m_Acceleration;   //‰Á‘¬“x
    float   m_Mass = 1.0f;    //¿—Ê
    float   m_Damping = 0.9f; //Œ¸ŠŒW”

public:

    // ŠO‚©‚ç—Í‚ğó‚¯æ‚é
    void AddVelocity(Vector3 force) { m_Velocity += force; }
    void AddForce(Vector3 force) { m_Acceleration += force / m_Mass; } // a = F / m

 
    void Update(float deltaTime) override;
};
