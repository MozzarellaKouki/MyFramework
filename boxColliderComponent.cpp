#include "boxColliderComponent.h"
#include "gameObject.h"

void BoxColliderComponent::Init()
{
    m_Transform =
        m_Owner->GetComponent<TransformComponent>();

    if (m_Transform == nullptr) return;
}



bool BoxColliderComponent::CheckCollision(
    ColliderComponent* other)
{
    BoxColliderComponent* box =
        dynamic_cast<BoxColliderComponent*>(other);

    if (box == nullptr) return false;

    Vector3 posA = GetPosition();
    Vector3 posB = box->GetPosition();

    Vector3 sizeA = GetWorldSize();
    Vector3 sizeB = box->GetWorldSize();

    float distanceX = fabsf(posA.x - posB.x);
    float distanceY = fabsf(posA.y - posB.y);
    float distanceZ = fabsf(posA.z - posB.z);

    if (distanceX > (sizeA.x + sizeB.x) * 0.5f)
        return false;

    if (distanceY > (sizeA.y + sizeB.y) * 0.5f)
        return false;

    if (distanceZ > (sizeA.z + sizeB.z) * 0.5f)
        return false;

    return true;
}