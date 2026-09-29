#pragma once
#include "component.h"
#include "mathUtils.h"

class ColliderComponent : public Component
{
public:
    virtual ~ColliderComponent() = default;

    virtual bool CheckCollision( ColliderComponent* other ) = 0;

};