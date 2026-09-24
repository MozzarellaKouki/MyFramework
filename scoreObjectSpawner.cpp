#include "scoreObjectSpawner.h"

#include "gameObjectManager.h"
#include "scoreObject3D.h"
#include "transformComponent.h"
#include "player3D.h"

#include <cstdlib>

void ScoreObjectSpawner::Init()
{
    m_Timer = 0.0f;
}

void ScoreObjectSpawner::Uninit()
{
}

void ScoreObjectSpawner::Update(float deltaTime)
{
    m_Timer += deltaTime;

    // 10秒経過したらスポーン
    if (m_Timer >= 10.0f)
    {
        m_Timer = 0.0f;

        int scoreObjectCount =  GameObjectManager::GetInstance().GetScoreObjectCount();
        if (scoreObjectCount >= 7) { return; }

        auto* scoreObject = GameObjectManager::GetInstance().CreateGameObject<ScoreObject3D>();


        float x = -300.0f + (rand() / (float)RAND_MAX) * 600.0f;
        float z = -300.0f + (rand() / (float)RAND_MAX) * 600.0f;

        auto* transform = scoreObject->GetComponent<TransformComponent>();

        if (transform != nullptr)
        {
            transform->SetPosition
            ({
                x,
                6.0f,
                z
            });
        }

        // プレイヤーを渡す
        scoreObject->SetPlayer(m_Player);
    }
}

void ScoreObjectSpawner::Draw()
{
    GameObject::Draw();
}