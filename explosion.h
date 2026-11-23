#pragma once
#include "spatialObject.h"
#include "spriteAnimationComponent.h"

class Explosion : public SpatialObject
{
private:

    SpriteAnimationComponent* m_Animation = nullptr;

public:

    void Init() override;
    void Update(float deltaTime) override;
};
