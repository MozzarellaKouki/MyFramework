#pragma once
#include "sceneManager.h"
#include "gameObject.h"
#include <vector>
#include <memory>


class Polygon2D;
class Camera;

class GameOverScene : public Scene
{

private:

    Polygon2D* m_Polygon;
    Camera* m_Camera;

    std::vector<std::unique_ptr<GameObject>> m_GameObjects;

public:

    //ゲームオブジェクトを生成
    template<typename T> T* CreateGameObject()
    {
        std::unique_ptr<T> obj = std::make_unique<T>();
        T* ptr = obj.get();
        m_GameObjects.push_back(std::move(obj));
        ptr->Init();

        return ptr;
    }


    void Init() override;

    void Update() override;

    void Draw() override;

    void Uninit() override;

};

