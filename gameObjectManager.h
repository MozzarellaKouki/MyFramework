#pragma once
#include "gameobject.h"
#include "sprite3DComponent.h"
#include <memory>
#include <vector>
#include <utility>

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
    bool TryGetGroundHeight(float worldX, float worldZ, float& worldHeight) const;

    
    static GameObjectManager& GetInstance()
    {
        static GameObjectManager instance;
        return instance;
    }

    template<typename T, typename... Args> T* CreateGameObject(Args&&... args)
    {
        std::unique_ptr<T> obj = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = obj.get();
        ptr->Init(); 
        m_PendingObjects.push_back(std::move(obj));

        return ptr;
    }

    std::vector<Enemy3D*> GetEnemies();
    int GetScoreObjectCount();


}; 
