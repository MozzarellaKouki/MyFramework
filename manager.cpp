#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2D.h"
#include "sceneManager.h"
#include "gameScene.h"
#include "titleScene.h"
#include "textureManager.h"
#include "audioManager.h"


void Manager::Init()
{
	Renderer::Init();

	Audio::InitMaster();
	AudioManager::Init();



	SceneManager::Init(std::make_unique<TitleScene>());

	TextureManager::Init();
}




void Manager::Uninit()
{
	SceneManager::Uninit();

	AudioManager::Uninit();
	Audio::UninitMaster();

	TextureManager::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	SceneManager::Update();
	AudioManager::Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	SceneManager::Draw();

	Renderer::End();
}
