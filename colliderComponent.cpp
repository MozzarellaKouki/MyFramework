#include "colliderComponent.h"
#include "collisionManager.h"
#include "gameObject.h"
#include "transformComponent.h"


ColliderComponent::~ColliderComponent()
{
    // Uninit を経由せずに破棄された場合の保険
    if (m_Registered)
    {
        CollisionManager::Unregister(this);
        m_Registered = false;
    }
}

void ColliderComponent::Init()
{
    GetTransform();

    if (!m_Registered)
    {
        CollisionManager::Register(this);
        m_Registered = true;
    }
}

void ColliderComponent::Uninit()
{
    if (m_Registered)
    {
        CollisionManager::Unregister(this);
        m_Registered = false;
    }
}

TransformComponent* ColliderComponent::GetTransform() const
{
    // コライダーが Transform より先に追加された場合にも対応するため遅延取得
    if (m_Transform == nullptr && m_Owner != nullptr)
    {
        m_Transform = m_Owner->GetComponent<TransformComponent>();
    }

    return m_Transform;
}

Vector3 ColliderComponent::GetWorldScale() const
{
    const TransformComponent* transform = GetTransform();
    return transform ? transform->GetScale() : Vector3(1.0f, 1.0f, 1.0f);
}

Vector3 ColliderComponent::GetCenter() const
{
    const TransformComponent* transform = GetTransform();
    if (transform == nullptr)
        return m_Offset;

    const Vector3 scale = transform->GetScale();
    const Vector3 offset(m_Offset.x * scale.x, m_Offset.y * scale.y, m_Offset.z * scale.z);
    return transform->GetPosition() + offset;
}
