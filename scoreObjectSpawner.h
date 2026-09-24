#pragma once

#include "gameObject.h"

class Player3D;

class ScoreObjectSpawner : public GameObject
{
private:
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
