#pragma once

#include "gameObject.h"
#include "particleEmitterComponent.h"

class Player3D;
class TransformComponent;
class PhysicsComponent;
class Sprite3DComponent;

class ScoreObject3D : public GameObject
{
private:
    TransformComponent* m_transform = nullptr;
    PhysicsComponent* m_physics = nullptr;
    Sprite3DComponent* m_sprite = nullptr;
    ParticleEmitterComponent* m_Emitter{};

    Player3D* m_Player = nullptr;

    float m_Timer = 0.0f;

public:
    void Init() override;
    void Uninit() override;
    void Update(float deltaTime) override;
    void Draw() override;

    void SetPlayer(Player3D* player)
    {
        m_Player = player;
    }
}; 
