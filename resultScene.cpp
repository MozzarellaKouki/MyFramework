#include "main.h"
#include "keyboard.h"
#include "resultScene.h"
#include "titleScene.h"
#include "gameObjectManager.h"
#include "textureManager.h"
#include "cameraManager.h"
#include "camera.h"
#include "renderer.h"	
#include "player3D.h"
#include "explosion.h"


#include "resultfield.h"



#include <vector>

void ResultScene::Init()
{

	//カメラの初期化
	m_Camera = new Camera();
	m_Camera->Init();

	auto* field2 =
		GameObjectManager::GetInstance()
		.CreateGameObject<Field2>();

	m_Camera->GetTransform()->SetPosition({ 0, 600, -10 });
	m_Camera->SetView({ 0,-90,0 });

	CameraManager::GetInstance().SetCamera(m_Camera);

}


void ResultScene::Uninit()
{
	GameObjectManager::GetInstance().Uninit();

	delete m_Camera;
	m_Camera = nullptr;

}

void ResultScene::Update()
{

	m_Camera->Update();
	GameObjectManager::GetInstance().Update(1.0f / 60.0f);

	
	if (Keyboard_IsKeyDownTrigger(KK_ENTER) || Keyboard_IsKeyDownTrigger(KK_SPACE))
	{
		SceneManager::ChangeScene(std::make_unique<TitleScene>());
	}


}

void ResultScene::Draw()
{

	m_Camera->Apply(); //オブジェクトを描写する前に呼ぶ

	Renderer::Set3D(); //3D描写
	GameObjectManager::GetInstance().Draw();
}
