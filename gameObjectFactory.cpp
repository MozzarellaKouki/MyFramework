#include "gameObjectFactory.h"

#include "gameObjectManager.h"
#include "player3D.h"
#include "enemy3D.h"
#include "field.h"
#include "scoreObjectSpawner.h"
#include "audioComponent.h"

Player3D* GameObjectFactory::CreatePlayer()
{
    return GameObjectManager::GetInstance().CreateGameObject<Player3D>();
}

Enemy3D* GameObjectFactory::CreateEnemy(Player3D* player)
{
    auto* enemy = GameObjectManager::GetInstance().CreateGameObject<Enemy3D>();
    enemy->SetPlayer(player);
    return enemy;
}

Field* GameObjectFactory::CreateField()
{
    return GameObjectManager::GetInstance().CreateGameObject<Field>();
}

Field* GameObjectFactory::CreateField(const HeightFieldConfig& config)
{
    return GameObjectManager::GetInstance().CreateGameObject<Field>(config);
}

ScoreObjectSpawner* GameObjectFactory::CreateScoreObjectSpawner(Player3D* player)
{
    auto* spawner = GameObjectManager::GetInstance()
        .CreateGameObject<ScoreObjectSpawner>();
    spawner->SetPlayer(player);
    return spawner;
}

GameObject* GameObjectFactory::CreateBGM()
{
    return GameObjectManager::GetInstance().CreateGameObject<GameObject>();
}