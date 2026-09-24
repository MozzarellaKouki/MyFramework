#include "main.h"
#include "keyboard.h"
#include "GameScene.h"
#include "resultScene.h"
#include "gameObjectManager.h"
#include "textureManager.h"
#include "field.h"
#include "cameraManager.h"
#include "camera.h"
#include "renderer.h"	
#include "player3D.h"
#include "explosion.h"
#include "audio.h"
#include "audioComponent.h"
#include "scoreManager.h"
#include "gameObjectFactory.h"
#include "audioManager.h"



#include "testMotion.h"



#include <vector>


void GameScene::Init()
{

	//カメラの初期化
	m_Camera = new Camera();
	m_Camera->Init();

	m_Skyplate.Init();
	m_Skyplate.SetCamera(m_Camera);

	m_NumberRenderer.Init();
	ScoreManager::Init(1);



	//auto* player3D = GameObjectFactory::CreatePlayer();
	m_Player = GameObjectFactory::CreatePlayer();
	auto* field = GameObjectFactory::CreateField();
	//auto* Enemy3D = GameObjectFactory::CreateEnemy(player3D);
	auto* scoreObjectSpawner = GameObjectFactory::CreateScoreObjectSpawner(m_Player);

	//GameObjectFactory::CreateBGM();
	AudioManager::PlayBGM
	(
		0,
		"Asset/Sound/BGM_clock.wav"
	);

	CameraManager::GetInstance().SetCamera(m_Camera);
	m_Camera->SetTarget(m_Player);



}


void GameScene::Uninit()
{
	GameObjectManager::GetInstance().Uninit();
	m_Skyplate.Uninit();

	delete m_Camera;
	m_Camera = nullptr;

	AudioManager::StopBGM();
	
}

void GameScene::Update()
{
	
	m_Camera->Update();
	GameObjectManager::GetInstance().Update(1.0f / 60.0f);

	m_Skyplate.Update(1.0f / 60.0f);


	int score = ScoreManager::GetScore(0);

	if (m_NextEnemyScore <= 20000 && score >= m_NextEnemyScore)
	{
		GameObjectFactory::CreateEnemy(m_Player);

		m_NextEnemyScore += 2000;
		if (m_NextEnemyScore >= 22000 && m_NextEnemyScore <= 24000) AudioManager::PlaySE(SEType::Enemy);
	}

	if (ScoreManager::GetScore(0) >= 30000)
	{
		SceneManager::ChangeSceneWhite(std::make_unique<ResultScene>());
		return;
	}


	// プレイヤーの位置を音のリスナー位置に設定
	auto* transform =
		m_Player->GetComponent<TransformComponent>();

	if (transform != nullptr)
	{
		AudioManager::SetListenerPosition(
			transform->GetPosition()
		);
	}

	if (transform != nullptr)
	{
		Vector3 pos = transform->GetPosition();

		if (pos.x < -1000.0f ||
			pos.x >  1000.0f ||
			pos.z < -1000.0f ||
			pos.z >	1000.0f)
		{
			SceneManager::QuitWithFade(0.06);
			return;
		}

		AudioManager::SetListenerPosition(pos);
	}



	//デバッグ用
	if (Keyboard_IsKeyDownTrigger(KK_B))
	{
		//SceneManager::ChangeScene(std::make_unique<ResultScene>());
	}

	if (Keyboard_IsKeyDownTrigger(KK_V))
	{
		//GameObjectFactory::CreateEnemy(m_Player);
	}


}

void GameScene::Draw()
{

	m_Camera->Apply(); //オブジェクトを描写する前に呼ぶ

	Renderer::Set3D(); //3D描写
	m_Skyplate.Draw();
	GameObjectManager::GetInstance().Draw();

	Renderer::Set2D(); //2D描写
	m_NumberRenderer.Draw(ScoreManager::GetScore(0), 20, 20);
}
