#pragma once

class GameObject;

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
};
