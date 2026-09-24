#include "gameObjectFactory.h"

#include "gameObjectManager.h"
#include "player3D.h"
#include "enemy3D.h"
#include "field.h"
#include "scoreObjectSpawner.h"
#include "audiocomponent.h"

Player3D* GameObjectFactory::CreatePlayer()
{
    auto* player =
        GameObjectManager::GetInstance()
        .CreateGameObject<Player3D>();

    return player;
}

Enemy3D* GameObjectFactory::CreateEnemy(Player3D* player)
{
    auto* enemy =
        GameObjectManager::GetInstance()
        .CreateGameObject<Enemy3D>();

    enemy->SetPlayer(player);

    return enemy;
}

Field* GameObjectFactory::CreateField()
{
    auto* field =
        GameObjectManager::GetInstance()
        .CreateGameObject<Field>();

    return field;
}

ScoreObjectSpawner* GameObjectFactory::CreateScoreObjectSpawner(Player3D* player)
{
    auto* spawner =
        GameObjectManager::GetInstance()
        .CreateGameObject<ScoreObjectSpawner>();

    spawner->SetPlayer(player);

    return spawner;
}


GameObject* GameObjectFactory::CreateBGM()
{
    auto* bgm =
        GameObjectManager::GetInstance()
        .CreateGameObject<GameObject>();

    auto* audio = bgm->AddComponent<AudioComponent>();

    audio->Load("Asset/Sound/BGM_clock.wav");
    audio->Play(true);

    return bgm;
}