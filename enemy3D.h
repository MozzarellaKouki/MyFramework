#pragma once

#include "gameObject.h"
#include "transformComponent.h"
#include "sprite3DComponent.h"
#include "physicsComponent.h"
#include "particleEmitterComponent.h"

enum class Enemy3DState
{
    None = 0,
    
    Normal,
};

class Enemy3D : public GameObject
{
private:
    TransformComponent* m_transform{};
    PhysicsComponent* m_physics{};
    Sprite3DComponent* m_sprite{};
    ParticleEmitterComponent* m_Emitter{};

    Enemy3DState m_State = Enemy3DState::None;

    GameObject* m_Player{};

    float m_MoveSpeed = 2.0f;
    float m_TeleportTimer = 0.0f;

public:
    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void Uninit() override;

    void SetState(Enemy3DState state)
    {
        m_State = state;
    }

    void SetPlayer(GameObject* player)
    {
        m_Player = player;
    }

    void SetMoveSpeed(float speed)
    {
        m_MoveSpeed = speed;
    }

private:
    void ChasePlayer(float deltaTime);
};