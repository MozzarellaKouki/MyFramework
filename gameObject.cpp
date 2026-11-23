#include "gameObject.h"

#include "debugUtils.h"


GameObject::GameObject()
    : m_Destroy(false)
{

}

void GameObject::Init()
{
    // コンポーネントは AddComponent 時に Init 済みなので、ここでは何もしない
}


void GameObject::Update(float deltaTime)
{
    for (auto& comp : m_Components)
    {
        comp->Update(deltaTime);
    }
}

void GameObject::Draw()
{
    for (auto& comp : m_Components)
    {
        comp->Draw();
    }
}

void GameObject::Uninit()
{
    UninitComponents();
}

void GameObject::UninitComponents()
{
    for (auto& comp : m_Components)
    {
        comp->Uninit();
    }

    m_Components.clear();
}
// 通知中にコンポーネントが追加されても安全なように、添字で回す
void GameObject::NotifyCollisionEnter(const Collision& collision)
{
    OnCollisionEnter(collision);

    const size_t count = m_Components.size();
    for (size_t i = 0; i < count && i < m_Components.size(); ++i)
    {
        m_Components[i]->OnCollisionEnter(collision);
    }
}

void GameObject::NotifyCollisionStay(const Collision& collision)
{
    OnCollisionStay(collision);

    const size_t count = m_Components.size();
    for (size_t i = 0; i < count && i < m_Components.size(); ++i)
    {
        m_Components[i]->OnCollisionStay(collision);
    }
}

void GameObject::NotifyCollisionExit(const Collision& collision)
{
    OnCollisionExit(collision);

    const size_t count = m_Components.size();
    for (size_t i = 0; i < count && i < m_Components.size(); ++i)
    {
        m_Components[i]->OnCollisionExit(collision);
    }
}
