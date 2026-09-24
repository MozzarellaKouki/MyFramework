#include "main.h"
#include "player3D.h"
#include "modelLoader.h"
#include "material.h"
#include "textureManager.h"
#include "keyboard.h"
#include "gameObjectManager.h"
#include "explosion.h"

#include "sprite3DComponent.h"
#include "particleEmitterComponent.h"
#include "particleTemperSettings.h"
#include "scoreManager.h"
#include "audioManager.h"

#include "debugUtils.h"

void Player3D::Init()
{

    m_transform = AddComponent<TransformComponent>();
    m_physics = AddComponent<PhysicsComponent>();
    m_renderer = AddComponent<MeshRendererComponent>();

    m_transform->SetPosition({ 0,0,0 });
    m_transform->SetRotation({ 0,0,0 });
    m_transform->SetScale({ 1,1,1 });
    
    SetState(Player3DState::Normal);

    //m_TestSE.Load("Asset/Sound/cursor_01.wav");



    //--------------------------------
    // OBJ読込
    //--------------------------------

    Mesh* mesh = ModelLoader::LoadOBJ("Asset/model/player.obj");
    assert(mesh != nullptr);
    m_renderer->SetMesh(mesh);

    //--------------------------------
    // Material
    //--------------------------------

    MaterialData mat{};

    mat.color =
    { 1,1,1,1 };

    mat.texture = TextureManager::Load(L"Asset/texture/Brick.png");

    m_renderer->SetMaterial(mat);


    //常駐パーティクル
    auto* emitter = AddComponent<ParticleEmitterComponent>();
    emitter->Configure(ParticleTemperSettings::Rain());
    emitter->SetBillboardType(BillboardType::Full);
    emitter->Play();

    m_SmokeEmitter = AddComponent<ParticleEmitterComponent>();
    m_SmokeEmitter->Configure(ParticleTemperSettings::Smoke());
    m_SmokeEmitter->SetBillboardType( BillboardType::Full );
}

void Player3D::Uninit()
{

}

void Player3D::Update(float deltaTime)
{

    switch (m_State)
    {

        case Player3DState::Normal:
        InputMove();
        break;
    }   


    if (Keyboard_IsKeyDownTrigger(KK_ENTER))
    {
        m_Boost = !m_Boost;
        AudioManager::PlaySE(SEType::Cursor);
        auto* emitter = AddComponent<ParticleEmitterComponent>();
        emitter->Configure(ParticleTemperSettings::ShockWave());
        emitter->SetBillboardType(BillboardType::Horizontal);
        emitter->Play(1.0f);

        if (m_Boost)
        {
            m_BoostFrameCount = 0;
            m_SmokeEmitter->Play();
        }
        else
        {
            m_SmokeEmitter->Stop();
        }
    }

    if (m_Boost)
    {
        m_BoostFrameCount++;

        if (m_BoostFrameCount >= 3)
        {
            m_BoostFrameCount = 0;

            if (ScoreManager::GetScore(0) > 0)
            {
                ScoreManager::AddScore(0, -1);
            }
        }
    }


    //デバッグ用　爆発エフェクト
    //if (Keyboard_IsKeyDownTrigger(KK_F))
    //{
    //  /*  auto* exp = GameObjectManager::GetInstance().CreateGameObject<Explosion>();
    //    exp->GetComponent<TransformComponent>()->SetPosition(m_transform->GetPosition());*/
    //    auto* emitter = AddComponent<ParticleEmitterComponent>();
    //    emitter->Configure(ParticleTemperSettings::ShockWave());
    //    emitter->SetBillboardType(BillboardType::Horizontal);
    //    emitter->Play(1.0f);

    //   // m_TestSE.Play(false);
    //   // AudioManager::PlaySE(SEType::Get);

    //   // ScoreManager::AddScore(0, 100);
    //}

    ////デバッグ用　パーティクル
    //if (Keyboard_IsKeyDownTrigger(KK_G))
    //{
    //    auto* emitter = AddComponent<ParticleEmitterComponent>();
    //    emitter->Configure(ParticleTemperSettings::Smoke());
    //    emitter->SetBillboardType(BillboardType::Full);
    //    emitter->Play(2.5f);

    //    //m_TestSE.Play(false);
    //    AudioManager::PlaySE(SEType::Enemy);

    //   // ScoreManager::AddScore(0, 800);
    //}
  



    GameObject::Update(deltaTime);

}

void Player3D::Draw()
{
    //OutputDebugStringA("Polygon3D Draw\n");

    GameObject::Draw();
}



void Player3D::InputMove()
{
  

    Vector3 rot = m_transform->GetRotation();
    float yaw = rot.y;

    Vector3 forward;
    forward.x = sinf(yaw);
    forward.y = 0.0f;
    forward.z = cosf(yaw);

    Vector3 right;
    right.x = cosf(yaw);
    right.y = 0.0f;
    right.z = -sinf(yaw);
    float movePower = m_Boost ? 160.0f : 80.0f;

    if (Keyboard_IsKeyDown(KK_W))
    {
        m_physics->AddForce(forward * movePower);
    }

    if (Keyboard_IsKeyDown(KK_S))
    {
        m_physics->AddForce(forward * -movePower);
    }

    if (Keyboard_IsKeyDown(KK_D))
    {
        m_physics->AddForce(right * movePower);
    }

    if (Keyboard_IsKeyDown(KK_A))
    {
        m_physics->AddForce(right * -movePower);
    }

    if (Keyboard_IsKeyDown(KK_RIGHT))
    {
        m_transform->AddRotation({ 0, 0.1f, 0 });
    }

    if (Keyboard_IsKeyDown(KK_LEFT))
    {
        m_transform->AddRotation({ 0, -0.1f, 0 });
    }


}