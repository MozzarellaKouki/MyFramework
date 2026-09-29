#pragma once

class GameObject;

class CollisionManager
{
public:
    static void CheckCollision( GameObject* objectA, GameObject* objectB);
};