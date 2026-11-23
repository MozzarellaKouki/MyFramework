#include "main.h"
#include "skyplate.h"

#include "camera.h"
#include "transform.h"
#include "textureManager.h"
#include "renderer.h"


void SkyPlate::Init()
{



    GetTransform()->SetPosition({ 0.0f, 0.0f, 0.0f });
    GetTransform()->SetRotation({ 0.0f, 0.0f, 0.0f });
    GetTransform()->SetScale({ 200.0f, 200.0f, 0.0f });

    m_Polygon.Init();



    auto* texture = TextureManager::Load( L"Asset/texture/Sky.png" );
    m_Polygon.SetTexture(texture);
}


void SkyPlate::Update(float deltaTime)
{
    if (m_Camera == nullptr)
        return;

    auto* cameraTransform =
        m_Camera->GetTransform();

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

    GetTransform()->SetPosition(pos);


    float yaw = atan2f
    (
        forward.x,
        forward.z
    );

    GetTransform()->SetRotation
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
        GetTransform()->GetWorldMatrix()
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