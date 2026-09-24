#include "main.h"
#include "keyboard.h"
#include "titleScene.h"
#include "gameScene.h"
#include "gameObjectManager.h"
#include "textureManager.h"
#include "cameraManager.h"
#include "camera.h"
#include "renderer.h"	
#include "player3D.h"
#include "explosion.h"

#include "titlefield.h"



#include <vector>

void TitleScene::Init()
{

	//カメラの初期化
	m_Camera = new Camera();
	m_Camera->Init();


	auto* field3 =
		GameObjectManager::GetInstance()
		.CreateGameObject<Field3>();

	m_Camera->GetTransform()->SetPosition({0, 600, -10});
	m_Camera->SetView({ 0,-90,0 });



	CameraManager::GetInstance().SetCamera(m_Camera);

}


void TitleScene::Uninit()
{
	GameObjectManager::GetInstance().Uninit();

	delete m_Camera;
	m_Camera = nullptr;

}

void TitleScene::Update()
{

	m_Camera->Update();
	GameObjectManager::GetInstance().Update(1.0f / 60.0f);

	//デバッグ用
	if (Keyboard_IsKeyDownTrigger(KK_ENTER) || Keyboard_IsKeyDownTrigger(KK_SPACE))
	{
		SceneManager::ChangeScene(std::make_unique<GameScene>());
	}
}

void TitleScene::Draw()
{

	m_Camera->Apply(); //オブジェクトを描写する前に呼ぶ

	Renderer::Set3D(); //3D描写
	GameObjectManager::GetInstance().Draw();
}
