#include "main.h"
#include "player.h"
#include "textureManager.h"
#include "keyboard.h"
#include "renderer.h"

void Player::Init()
{
    m_Sprite = new Polygon2D();
    m_Sprite->Init();

    auto tex = TextureManager::Load(L"Asset/texture/Brick.png");
    m_Sprite->SetTexture(tex);

   
    m_Transform.position = { 0,0,0 };
    m_Transform.rotation = { 0,0,0 };
    m_Transform.scale = { 1,1,1 };

    m_State = PlayerState::Normal;

    m_Velocity = { 0,0,0 };
}

void Player::Uninit()
{
    if (m_Sprite)
    {
        m_Sprite->Uninit();
        delete m_Sprite;
        m_Sprite = nullptr;
    }


}

void Player::Update()
{
    switch (m_State)
    {

    case PlayerState::Normal:
        InputMove();
        break;

    }


    SetPosition(m_Velocity);

}

void Player::Draw()
{
    Renderer::SetWorldMatrix(m_Transform.GetWorldMatrix());

    m_Sprite->Draw();
}





void Player::ChangeState(PlayerState state)
{
    
}

void Player::InputMove()
{
 
    if (Keyboard_IsKeyDown(KK_D))
    {
        m_Velocity.x = 1.0f;
    }

    if (Keyboard_IsKeyDown(KK_A))
    {
        m_Velocity.x = -1.0f;
    }

    if (Keyboard_IsKeyDown(KK_S))
    {
        m_Velocity.y = 1.0f;
    }

    if (Keyboard_IsKeyDown(KK_W))
    {
        m_Velocity.y = -1.0f;
    }
}


