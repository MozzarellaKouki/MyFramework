#pragma once
#include "sceneManager.h"
#include "gameObject.h"
#include <vector>
#include <memory>
#include "numberRenderer.h"
#include "audio.h"
#include "skyPlate.h"
#include "player3D.h"


#include "audioComponent.h"

class Polygon2D;
class Camera;

class GameScene : public Scene
{

private:

    //Polygon2D* m_Polygon;
    GameObject* m_BGMObject = nullptr;
    Camera* m_Camera;
    SkyPlate m_Skyplate;
    std::vector<std::unique_ptr<GameObject>> m_GameObjects;
   

    //Test
    int m_Score = 0;
    int m_NextEnemyScore = 2000;

    NumberRenderer m_NumberRenderer;
    Audio m_TestSE;
    Player3D* m_Player = nullptr;

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
