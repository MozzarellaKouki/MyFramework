#include "collisionManager.h"
#include "gameObject.h"
#include "colliderComponent.h"

#include <iostream>

void CollisionManager::CheckCollision( GameObject* objectA, GameObject* objectB)
{
    if (objectA == nullptr || objectB == nullptr)
        return;

    auto* colliderA = objectA->GetComponent<ColliderComponent>();
    auto* colliderB = objectB->GetComponent<ColliderComponent>();

    if (colliderA == nullptr || colliderB == nullptr) return;

    if (colliderA->CheckCollision(colliderB))
    {
        // Õ“Ë‚µ‚½‚Æ‚«‚Ìˆ—
        std::cout << "COLLISION!!\n";
    }


}