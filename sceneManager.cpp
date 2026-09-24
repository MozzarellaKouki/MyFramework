#include "SceneManager.h"
#include "renderer.h"


std::unique_ptr<Scene> SceneManager::currentScene = nullptr;
std::unique_ptr<Scene> SceneManager::nextScene = nullptr;
FadeObject SceneManager::fadeObject;
bool SceneManager::m_SceneChanging = false;
bool SceneManager::m_QuitWithFade = false;

void SceneManager::Init(std::unique_ptr<Scene> scene)
{
    fadeObject.Init();
    currentScene = std::move(scene);
    currentScene->Init();
}


void SceneManager::ChangeScene(std::unique_ptr<Scene> scene)
{
    nextScene = std::move(scene);
    m_SceneChanging = true;
    fadeObject.FadeOut();
}

void SceneManager::ChangeSceneWhite(std::unique_ptr<Scene> scene)
{
    nextScene = std::move(scene);
    m_SceneChanging = true;
    fadeObject.FadeOutWhite();
}

void SceneManager::ChangeSceneRed(std::unique_ptr<Scene> scene)
{
    nextScene = std::move(scene);
    m_SceneChanging = true;
    fadeObject.FadeOutRed();
}

void SceneManager::QuitWithFade(float speed)
{
    if (m_QuitWithFade)
        return;

    m_QuitWithFade = true;
    fadeObject.FadeOut(speed);
}

void SceneManager::Update()
{

    if (currentScene && !m_SceneChanging)
    {
        currentScene->Update();
    }

    fadeObject.Update(1.0f / 60.0f);

    if (m_SceneChanging &&
        !fadeObject.IsFading() &&
        !fadeObject.IsFadeIn())
    {
        if (currentScene)
        {
            currentScene->Uninit();
        }

        currentScene = std::move(nextScene);
        currentScene->Init();

        fadeObject.FadeIn();

        m_SceneChanging = false;
    }

    if (m_QuitWithFade &&
        !fadeObject.IsFading() &&
        !fadeObject.IsFadeIn())
    {
        PostQuitMessage(0);
        return;
    }

}


void SceneManager::Draw()
{
    if (currentScene) currentScene->Draw();

    Renderer::Set2D();
    fadeObject.Draw();
}


void SceneManager::Uninit()
{
    fadeObject.Uninit();

    if (currentScene)
    {
        currentScene->Uninit();
        currentScene = nullptr;
    }
}