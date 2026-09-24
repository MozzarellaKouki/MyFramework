#include "gameObjectManager.h"

#include "sprite3DComponent.h"
#include "particleEmitterComponent.h"
#include "TransparentRenderManager.h"
#include "enemy3D.h"
#include "scoreObject3D.h"


void GameObjectManager::Uninit()
{
    for (auto& obj : m_GameObjects)
    {
        obj->Uninit();
    }

    m_GameObjects.clear();
}


void GameObjectManager::Update(float dt)
{
    for (auto& obj : m_GameObjects)
    {
        obj->Update(dt);
    }

    //生成予約されたオブジェクトの処理
    for (auto& obj : m_PendingObjects)
    {
        m_GameObjects.push_back(std::move(obj));
    }

    m_PendingObjects.clear();

    //削除予約されたオブジェクトの処理
    for (auto it = m_GameObjects.begin();
        it != m_GameObjects.end();)
    {
        if ((*it)->IsDestroy())
        {
            (*it)->Uninit();

            it = m_GameObjects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


void GameObjectManager::Draw()
{
    // 通常描画
    for (auto& obj : m_GameObjects)
    {
        obj->Draw();
    }

    // 透明描画
    Renderer::SetATCEnable(true);
    Renderer::SetDepthEnable(false);

    TransparentRenderManager::Begin();

    Sprite3DComponent::SubmitTransparentSprites();
    ParticleEmitterComponent::SubmitTransparentParticles();

    TransparentRenderManager::Draw();

    Renderer::SetDepthEnable(true);
    Renderer::SetATCEnable(false);

}



std::vector<Enemy3D*> GameObjectManager::GetEnemies()
{
    std::vector<Enemy3D*> enemies;

    for (auto& obj : m_GameObjects)
    {
        Enemy3D* enemy =
            dynamic_cast<Enemy3D*>(obj.get());

        if (enemy != nullptr)
        {
            enemies.push_back(enemy);
        }
    }

    return enemies;
}

int GameObjectManager::GetScoreObjectCount()
{
    int count = 0;

    for (auto& obj : m_GameObjects)
    {
        if (dynamic_cast<ScoreObject3D*>(obj.get()) != nullptr)
        {
            count++;
        }
    }

    return count;
}

