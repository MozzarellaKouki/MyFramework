#include "main.h"
#include "skyplate.h"

#include "camera.h"
#include "transformComponent.h"
#include "textureManager.h"
#include "renderer.h"


void SkyPlate::Init()
{


    m_Transform = AddComponent<TransformComponent>();

    m_Transform->SetPosition({ 0.0f, 0.0f, 0.0f });
    m_Transform->SetRotation({ 0.0f, 0.0f, 0.0f });
    m_Transform->SetScale({ 200.0f, 200.0f, 0.0f });

    m_Polygon.Init();



    auto* texture = TextureManager::Load( L"Asset/texture/Sky.png" );
    m_Polygon.SetTexture(texture);
}


void SkyPlate::Update(float deltaTime)
{
    if (m_Camera == nullptr)
        return;

    auto* cameraTransform =
        m_Camera->GetComponent<TransformComponent>();

    if (cameraTransform == nullptr)
        return;

    Vector3 cameraPos =
        cameraTransform->GetPosition();

    Vector3 forward =
        m_Camera->GetForward();

   

    Vector3 pos;

    pos.x = cameraPos.x + forward.x * 100.0f;
    pos.y = cameraPos.y + forward.y * 100.0f - 20;
    pos.z = cameraPos.z + forward.z * 100.0f;

    m_Transform->SetPosition(pos);


    float yaw = atan2f
    (
        forward.x,
        forward.z
    );

    m_Transform->SetRotation
    ({
        0.0f,
        yaw,
        0.0f
       
    });
}

void SkyPlate::Draw()
{
    if (m_Camera == nullptr)  return;


    SPRITE_ANIMATION anim{};
    anim.Frame = 0;
    anim.Columns = 1;
    anim.Rows = 1;
    Renderer::SetSpriteAnimation(anim);


    Renderer::SetWorldMatrix(
        m_Transform->GetWorldMatrix()
    );


    m_Polygon.Draw();
}


void SkyPlate::Uninit()
{
    m_Polygon.Uninit();
}


void SkyPlate::SetCamera(Camera* camera)
{
    m_Camera = camera;
}