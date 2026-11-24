#pragma once

class GameObject;
class Transform;
class SpatialObject;
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

    //持ち主を SpatialObject として取得する（Init で1回だけ呼んでキャッシュすること）
    //FindSpatialOwner    : SpatialObject でなければ nullptr（位置がなくても動くコンポーネント用）
    //RequireSpatialOwner : SpatialObject でなければエラーを出して nullptr（位置が必須のコンポーネント用）
    SpatialObject* FindSpatialOwner() const;
    SpatialObject* RequireSpatialOwner(const char* componentName) const;

    //RequireSpatialOwner の Transform 版
    Transform* RequireTransform(const char* componentName) const;
};
