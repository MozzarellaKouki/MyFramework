#pragma once
#include "SceneBase.h"
#include "fadeObject.h"
#include <memory>


class SceneManager
{
private:

    static std::unique_ptr<Scene> currentScene;
    static std::unique_ptr<Scene> nextScene;

    static FadeObject fadeObject;
    static bool m_SceneChanging;
    static bool m_QuitWithFade;
   

public:

  
    static void Init(std::unique_ptr<Scene> scene);

    static void ChangeScene(std::unique_ptr<Scene> scene);
    static void ChangeSceneWhite(std::unique_ptr<Scene> scene);
    static void ChangeSceneRed(std::unique_ptr<Scene> scene);
    static void QuitWithFade(float speed);

    static void Update();
    static void Draw();
    static void Uninit();

};