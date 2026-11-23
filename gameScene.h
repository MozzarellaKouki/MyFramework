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

    Camera* m_Camera = nullptr;
    SkyPlate m_Skyplate;

    //Test
    int m_NextEnemyScore = 2000;

    NumberRenderer m_NumberRenderer;
    Player3D* m_Player = nullptr;

    void RegisterDebugCommands();

public:

    void Init() override;

    void Update() override;

    void Draw() override;

    void Uninit() override;

};
