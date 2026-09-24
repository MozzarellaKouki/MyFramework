#pragma once
#include "gameobject.h"
#include "sprite3DComponent.h"
#include <memory>
#include <vector>

class Enemy3D;


class GameObjectManager
{
private:

    std::vector<std::unique_ptr<GameObject>> m_GameObjects;
    std::vector<std::unique_ptr<GameObject>> m_PendingObjects;
  


public:

    void Uninit();
    void Update(float dt);
    void Draw();

    
    static GameObjectManager& GetInstance()
    {
        static GameObjectManager instance;
        return instance;
    }

    template<typename T> T* CreateGameObject()
    {
        std::unique_ptr<T> obj = std::make_unique<T>();
        T* ptr = obj.get();
        ptr->Init(); 
        m_PendingObjects.push_back(std::move(obj));

        return ptr;
    }

    std::vector<Enemy3D*> GetEnemies();
    int GetScoreObjectCount();


}; 
