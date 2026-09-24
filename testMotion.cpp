#include "main.h"
#include "testMotion.h"
#include "textureManager.h"
#include "keyboard.h"
#include "renderer.h"


static Transform g_InitialPosotion;
static float g_SpeedUpSpeed;
static bool g_IsSpeedUp;




void TestMotion::Init()
{
    m_Sprite = new Polygon2D();
    m_Sprite->Init();

    auto tex = TextureManager::Load(L"Asset/texture/BlueDonut.png");
    m_Sprite->SetTexture(tex);


    g_InitialPosotion.position = { SCREEN_WIDTH / 2 -90 ,SCREEN_HEIGHT / 2 -90  ,0};
    g_SpeedUpSpeed = 3.0f;
    g_IsSpeedUp = false;

    m_Transform.scale = { 0.5,0.5,0 };
    m_State = State::UniformVelocity;
    m_Velocity = { 0,0,0 };


    m_Time = 0.0f;               //t
    m_Speed = 1.0f;              //s
    m_Acceleration = 0.2f;       //a
    m_Grabity = 0.98f;           //g
    m_InitialVelocity = 25.0f;   //v
    m_Degree = 45.0f;            //θ
  
}

void TestMotion::Uninit()
{
    if (m_Sprite)
    {
        m_Sprite->Uninit();
        delete m_Sprite;
        m_Sprite = nullptr;
    }


}

void TestMotion::Update()
{
    Input();

    switch (m_State)
    {      

    case State::UniformVelocity:

        m_Transform.position.x = (g_InitialPosotion.position.x + (m_Time * m_Speed));
        m_Transform.position.y = (g_InitialPosotion.position.y);

        break;


    case State::UniformAcceleration:

        m_Transform.position.x = (g_InitialPosotion.position.x + (m_Time * m_Speed) + (0.5f * m_Acceleration * m_Time * m_Time));
        m_Transform.position.y = (g_InitialPosotion.position.y);
     

        break;

    case State::FreeFall:

        m_Transform.position.x = (g_InitialPosotion.position.x);
        m_Transform.position.y = (g_InitialPosotion.position.y + (0.5f * m_Grabity * m_Time * m_Time));

        break;


    case State::VerticalProjectile:

        m_Transform.position.x = (g_InitialPosotion.position.x);
        m_Transform.position.y = (g_InitialPosotion.position.y - (m_InitialVelocity * m_Time) + (0.5f * m_Grabity * m_Time * m_Time));

        break;


    case State::HorizontalProjectile:

        m_Transform.position.x = (g_InitialPosotion.position.x + (m_Time * m_InitialVelocity));
        m_Transform.position.y = (g_InitialPosotion.position.y + (0.5f * m_Grabity * m_Time * m_Time));

        break;


    case State::ObliqueProjectile:

        float rad = DegToRad(m_Degree);

        m_Transform.position.x = (g_InitialPosotion.position.x + (m_InitialVelocity * cos(rad)) * m_Time);
        m_Transform.position.y = (g_InitialPosotion.position.y - ((m_InitialVelocity * sin(rad)) * m_Time) + (0.5f * m_Grabity * m_Time * m_Time));

        break;

    }

    RoopCheck();


    //ウインドウ名にデバッグ教示
    char text[256];
    sprintf_s(text, "Time = %f  State = %d", m_Time, (int)m_State);
    SetWindowText(GetWindow(), text);

    SetWindowText(GetWindow(), text);


}

void TestMotion::Draw()
{
    Renderer::SetWorldMatrix(m_Transform.GetWorldMatrix());

    m_Sprite->Draw();
}


void TestMotion::Input()
{

    if (Keyboard_IsKeyDownTrigger(KK_SPACE))
    {
        g_IsSpeedUp = !g_IsSpeedUp;
    }


    if (Keyboard_IsKeyDown(KK_D))
    {
        float t = g_IsSpeedUp ? g_SpeedUpSpeed : 1.0f;
        m_Time += 1.0f * t;
    }
      

    if (Keyboard_IsKeyDown(KK_A))
    {
        float t = g_IsSpeedUp ? g_SpeedUpSpeed : 1.0f;
        m_Time -= 1.0f * t;
    }
       

    if (Keyboard_IsKeyDown(KK_W))
    {
        m_Time = 0;
    }



    //状態変更
    if (Keyboard_IsKeyDownTrigger(KK_D1))
        m_State = State::UniformVelocity;

    if (Keyboard_IsKeyDownTrigger(KK_D2))
        m_State = State::UniformAcceleration;

    if (Keyboard_IsKeyDownTrigger(KK_D3))
        m_State = State::FreeFall;

    if (Keyboard_IsKeyDownTrigger(KK_D4))
        m_State = State::VerticalProjectile;

    if (Keyboard_IsKeyDownTrigger(KK_D5))
        m_State = State::HorizontalProjectile;

    if (Keyboard_IsKeyDownTrigger(KK_D6))
        m_State = State::ObliqueProjectile;

}

void TestMotion::RoopCheck()
{
    int loopCountX = 0;
    int loopCountY = 0;
    const int LOOP_LIMIT = 400;

   while (m_Transform.position.x > SCREEN_WIDTH ) 
   {
       m_Transform.position.x -= (SCREEN_WIDTH + 110); 
       if (++loopCountX > LOOP_LIMIT) { break; }
   }

   while (m_Transform.position.y > SCREEN_HEIGHT ) 
   {
       m_Transform.position.y -= (SCREEN_HEIGHT + 110);
       if (++loopCountY > LOOP_LIMIT) { break; }
   }

   while (m_Transform.position.x < -110) 
   { 
       m_Transform.position.x += SCREEN_WIDTH + 90; 
       if (++loopCountX > LOOP_LIMIT) { break; }
   }

   while (m_Transform.position.y < -110) 
   {
       m_Transform.position.y += SCREEN_HEIGHT + 90;
       if (++loopCountY > LOOP_LIMIT)  { break; }
   }
}