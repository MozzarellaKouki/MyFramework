#pragma once
#include "colliderComponent.h"
#include "mathUtils.h"

//--------------------------------------------------------------
// ボックスコライダー（軸平行 AABB）
// サイズは Transform のスケールが掛かる。回転は無視される。
//--------------------------------------------------------------
class BoxColliderComponent : public ColliderComponent
{
private:

    Vector3 m_Size = { 1.0f, 1.0f, 1.0f };

public:

    BoxColliderComponent() : ColliderComponent(ColliderShape::Box) {}

    void SetSize(const Vector3& size) { m_Size = size; }
    Vector3 GetSize() const { return m_Size; }

    // ワールド空間での中心
    Vector3 GetPosition() const { return GetCenter(); }

    // スケール込みのサイズ
    Vector3 GetWorldSize() const
    {
        const Vector3 scale = GetWorldScale();
        return { m_Size.x * scale.x, m_Size.y * scale.y, m_Size.z * scale.z };
    }

    Vector3 GetHalfExtents() const { return GetWorldSize() * 0.5f; }
};
