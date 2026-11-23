#include "scoreObject3D.h"

#include "transformComponent.h"
#include "physicsComponent.h"
#include "sprite3DComponent.h"
#include "player3D.h"
#include "scoreManager.h"
#include "gameObjectManager.h"
#include "audioManager.h"

#include "particleEmitterComponent.h"
#include "particleTemperSettings.h"
#include "sphereColliderComponent.h"

#include "keyboard.h"

void ScoreObject3D::Init()
{
    m_transform = AddComponent<TransformComponent>();
    m_physics = AddComponent<PhysicsComponent>();
    m_sprite = AddComponent<Sprite3DComponent>();

    m_transform->SetPosition({ 0, 0, 0 });
    m_transform->SetRotation({ 0, 0, 0 });
    m_transform->SetScale({ 10, 10, 10 });

    m_sprite->SetSprite( L"Asset/Texture/2DBillboard/cookie.png" );

    m_sprite->SetBillboard(BillboardType::Full);
    m_sprite->SetSortByDistance(true);

    m_Emitter = AddComponent<ParticleEmitterComponent>();
    m_Emitter->Configure( ParticleTemperSettings::ShockWave() );
    m_Emitter->SetBillboardType( BillboardType::Horizontal );

    // 取得判定（半径 0.8 × スケール10 = 約8）
    auto* collider = AddComponent<SphereColliderComponent>();
    collider->SetRadius(0.8f);
    collider->SetLayer(CollisionLayer::Item);
    collider->SetMask(CollisionLayer::Player);
}

void ScoreObject3D::Uninit()
{
}

void ScoreObject3D::Update(float deltaTime)
{
    if (m_Player == nullptr)
        return;

    m_Timer += deltaTime;
    if (m_Timer >= 5.0f)
    {
        m_Timer = 0.0f;

        auto* emitter = AddComponent<ParticleEmitterComponent>();
        emitter->Configure(ParticleTemperSettings::Smoke());
        emitter->SetBillboardType(BillboardType::Full);
        emitter->Play(1.5f);
    }
}

void ScoreObject3D::OnCollisionEnter(const Collision& collision)
{
    // マスクで Player だけと判定しているので、ここに来たら取得
    ScoreManager::AddScore(0, 1000);
    AudioManager::PlaySE(SEType::Get);
    Destroy();
}

void ScoreObject3D::Draw()
{
    GameObject::Draw();
}