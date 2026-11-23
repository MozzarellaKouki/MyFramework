#include "gameObjectManager.h"

#include "sprite3DComponent.h"
#include "particleEmitterComponent.h"
#include "TransparentRenderManager.h"
#include "enemy3D.h"
#include "scoreObject3D.h"


#include "debugUtils.h"
#include "field.h"
#include "collisionManager.h"



void GameObjectManager::Uninit()
{
    // 破棄中に OnCollisionExit が飛ばないよう、先に衝突状態を消す
    CollisionManager::Clear();

    for (auto& obj : m_GameObjects)
    {
        obj->Uninit();
    }

    // まだ Update に乗っていない生成予約分も破棄する（シーン切替時の取り残し防止）
    for (auto& obj : m_PendingObjects)
    {
        obj->Uninit();
    }

    m_GameObjects.clear();
    m_PendingObjects.clear();
}


void GameObjectManager::Update(float dt)
{
    for (auto& obj : m_GameObjects)
    {
        obj->Update(dt);
    }

    // 当たり判定（Destroy 予約されたオブジェクトはここで Exit が通知される）
    CollisionManager::Update();

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

#include "heightFieldSurfaceComponent.h"

bool GameObjectManager::TryGetGroundHeight(
    float worldX,
    float worldZ,
    float& worldHeight) const
{
    for (const auto& obj : m_GameObjects)
    {
        if (!obj || obj->IsDestroy())
            continue;

        const auto* surface = obj->GetComponent<HeightFieldSurfaceComponent>();
        if (surface && surface->TryGetHeight(worldX, worldZ, worldHeight))
            return true;
    }

    return false;
}
