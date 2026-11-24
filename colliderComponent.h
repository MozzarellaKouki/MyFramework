#pragma once
#include "component.h"
#include "collision.h"
#include "mathUtils.h"

class Transform;

enum class ColliderShape
{
    Box,    // 軸平行ボックス（AABB）。回転は無視される
    Sphere,
};

//--------------------------------------------------------------
// コライダー基底クラス
//
// AddComponent した時点で CollisionManager に自動登録され、
// Uninit / 破棄時に自動で登録解除される。
// SpatialObject（位置を持つオブジェクト）に追加すること。
//
// 衝突は GameObject / Component の OnCollisionEnter / Stay / Exit で受け取る。
// ※コールバック内でオブジェクトを消すときは Uninit ではなく Destroy() を使う。
//--------------------------------------------------------------
class ColliderComponent : public Component
{
public:

    explicit ColliderComponent(ColliderShape shape) : m_Shape(shape) {}
    ~ColliderComponent() override;

    void Init() override;
    void Uninit() override;

    ColliderShape GetShape() const { return m_Shape; }

    // 中心のオフセット（ローカル。Transform のスケールが掛かる）
    void SetOffset(const Vector3& offset) { m_Offset = offset; }
    Vector3 GetOffset() const { return m_Offset; }

    // ワールド空間での中心
    Vector3 GetCenter() const;

    // 自分が属するレイヤーと、判定する相手のレイヤー
    void SetLayer(uint32_t layer) { m_Layer = layer; }
    void SetMask(uint32_t mask) { m_Mask = mask; }
    uint32_t GetLayer() const { return m_Layer; }
    uint32_t GetMask() const { return m_Mask; }

    void SetEnabled(bool enabled) { m_Enabled = enabled; }
    bool IsEnabled() const { return m_Enabled; }

    bool IsRegistered() const { return m_Registered; }

    //持ち主（Init 以降に有効）
    SpatialObject* GetSpatialOwner() const { return m_SpatialOwner; }

protected:

    const Transform* GetTransform() const { return m_Transform; }
    Vector3 GetWorldScale() const;

private:

    ColliderShape m_Shape;
    Vector3  m_Offset = { 0.0f, 0.0f, 0.0f };
    uint32_t m_Layer = CollisionLayer::Default;
    uint32_t m_Mask = CollisionLayer::All;
    bool     m_Enabled = true;
    bool     m_Registered = false;

    SpatialObject* m_SpatialOwner = nullptr;
    Transform* m_Transform = nullptr;
};
