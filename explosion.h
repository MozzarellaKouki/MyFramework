#pragma once
#include "gameObject.h"
#include "spriteAnimationComponent.h"

class Explosion : public GameObject
{
private:

    SpriteAnimationComponent* m_Animation = nullptr;

public:

    void Init() override;
    void Update(float deltaTime) override;
};
