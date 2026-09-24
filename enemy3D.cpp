#include "enemy3D.h"
#include "sprite3DComponent.h"
#include "sceneManager.h"
#include "GameoverScene.h"
#include "particleTemperSettings.h"
#include "scoreManager.h"

void Enemy3D::Init()
{
    m_transform = AddComponent<TransformComponent>();
    m_physics = AddComponent<PhysicsComponent>();
    m_sprite = AddComponent<Sprite3DComponent>();
    m_State = Enemy3DState::Normal;


    m_transform->SetPosition({ 0, 3, 10 });
    m_transform->SetRotation({ 0,0,0 });
    m_transform->SetScale({ 10, 10, 10 });

    Enemy3D::SetMoveSpeed(5.9f);

    m_sprite->SetSprite(L"Asset/Texture/2DBillboard/Eye.png");
    m_sprite->SetBillboard(BillboardType::Full);
    m_sprite->SetSortByDistance(true);

    m_Emitter = AddComponent<ParticleEmitterComponent>();
    m_Emitter->Configure(ParticleTemperSettings::ShockWave());
    m_Emitter->SetBillboardType(BillboardType::Horizontal);
}

void Enemy3D::Uninit()
{

}

void Enemy3D::Update(float deltaTime)
{
    if (m_State == Enemy3DState::Normal)
    {
        ChasePlayer(deltaTime);
    }

    if (m_transform->GetPosition().y < 3 )  
    {
        Vector3 pos = m_transform->GetPosition();
        pos.y = 3.0f;
        m_transform->SetPosition(pos);
    }



    m_TeleportTimer += deltaTime;
    if (m_TeleportTimer >= 5.0f)
    {
        m_TeleportTimer = 0.0f;

        int random = rand() % 100;

        if (random < 8)
        {
            Vector3 pos = m_transform->GetPosition();

            pos.x = -300.0f +
                (rand() / (float)RAND_MAX) * 600.0f;

            pos.z = -300.0f +
                (rand() / (float)RAND_MAX) * 600.0f;

            pos.y = 3.0f;

            m_transform->SetPosition(pos);
            m_Emitter->Play(3.0f);
        }
    }
}

void Enemy3D::Draw()
{
    if (m_Player == nullptr)
        return;

    auto* playerTransform = m_Player->GetComponent<TransformComponent>();

    if (playerTransform == nullptr)
        return;

    Vector3 enemyPosition =
        m_transform->GetPosition();

    Vector3 playerPosition =
        playerTransform->GetPosition();

    Vector3 difference =
        enemyPosition - playerPosition;

    float distance = sqrt
    (
        difference.x * difference.x +
        difference.y * difference.y +
        difference.z * difference.z
    );


    m_sprite->SetVisible(distance <= 25.0f);
}


void Enemy3D::ChasePlayer(float deltaTime)
{
    if (m_Player == nullptr)
        return;

    TransformComponent* playerTransform =
        m_Player->GetComponent<TransformComponent>();

    if (playerTransform == nullptr)
        return;

    Vector3 enemyPosition = m_transform->GetPosition();
    Vector3 playerPosition = playerTransform->GetPosition();
    Vector3 direction = playerPosition - enemyPosition;


    float distance = sqrt
    (
            direction.x * direction.x +
            direction.y * direction.y +
            direction.z * direction.z
    );
    if (distance < 0.001f)  return;


    direction /= distance;

    float moveSpeed = m_MoveSpeed;

    if (ScoreManager::GetScore(0) >= 22000)
    {
        moveSpeed *= 1.5f;
    }

    enemyPosition += direction * moveSpeed * deltaTime;
    m_transform->SetPosition(enemyPosition);


    if (distance < 4.0f)
    {
        SceneManager::ChangeSceneRed( std::make_unique<GameOverScene>());

        return;
    }
}