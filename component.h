#pragma once

class GameObject;
class Transform;
struct Collision;

class Component
{
protected:

    GameObject* m_Owner{};

public:

    virtual ~Component() = default;

    virtual void Init() {}
    virtual void Update(float dt) {}
    virtual void Draw() {}
    virtual void Uninit() {}

    // 当たり判定の通知（同じ GameObject のどれかのコライダーが衝突した時）
    virtual void OnCollisionEnter(const Collision& collision) {}
    virtual void OnCollisionStay(const Collision& collision) {}
    virtual void OnCollisionExit(const Collision& collision) {}

    void SetOwner(GameObject* owner)
    {
        m_Owner = owner;
    }

    GameObject* GetOwner()
    {
        return m_Owner;
    }

    const GameObject* GetOwner() const
    {
        return m_Owner;
    }

protected:

    // 位置が必要なコンポーネントが Init で呼ぶ。
    // 持ち主が SpatialObject でなければ（位置を持たなければ）エラーを出して nullptr を返す。
    Transform* RequireTransform(const char* componentName) const;
};
