#pragma once
#include "colliderComponent.h"
#include <algorithm>
#include <cmath>

//--------------------------------------------------------------
// 球コライダー
// 半径には Transform のスケールの最大成分が掛かる。
//--------------------------------------------------------------
class SphereColliderComponent : public ColliderComponent
{
private:

    float m_Radius = 0.5f;

public:

    SphereColliderComponent() : ColliderComponent(ColliderShape::Sphere) {}

    void SetRadius(float radius) { m_Radius = radius; }
    float GetRadius() const { return m_Radius; }

    // スケール込みの半径
    float GetWorldRadius() const
    {
        const Vector3 s = GetWorldScale();
        const float maxScale = (std::max)({ std::fabs(s.x), std::fabs(s.y), std::fabs(s.z) });
        return m_Radius * maxScale;
    }
};
