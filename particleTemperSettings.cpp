#include "ParticleTemperSettings.h"

ParticleTemper ParticleTemperSettings::Smoke()
{
    ParticleTemper t;

    t.TexturePath = { L"Asset/Texture/Effect/Smoke_1.png", 
                      L"Asset/Texture/Effect/Smoke_2.png", };

    t.MinLife = 1.5f;
    t.MaxLife = 2.5f;

    t.EmitRate = 60.0f;

    t.MinVelocity = { -0.5f, 1.0f, -0.5f };
    t.MaxVelocity = { 0.5f, 3.0f,  0.5f };

    t.Acceleration = { 0.0f, 1.5f, 0.0f };   //è≠Çµè„è∏


    t.MinStartScale = 0.3f;
    t.MaxStartScale = 0.4f;
    t.MinEndScale = 2.2f;
    t.MaxEndScale = 3.0f;
   

    return t;
}

ParticleTemper ParticleTemperSettings::Rain()
{
    ParticleTemper t{};

    t.TexturePath = { L"Asset/Texture/Effect/furit_1.png",
                      L"Asset/Texture/Effect/furit_2.png", 
                      L"Asset/Texture/Effect/furit_3.png", 
                      L"Asset/Texture/Effect/furit_4.png", 
                      L"Asset/Texture/Effect/furit_5.png", 
                      L"Asset/Texture/Effect/furit_6.png", 
                      L"Asset/Texture/Effect/furit_7.png", 
                      L"Asset/Texture/Effect/furit_8.png", 
                      L"Asset/Texture/Effect/furit_9.png", 
                      L"Asset/Texture/Effect/furit_10.png", 
                      L"Asset/Texture/Effect/furit_11.png", 
                      L"Asset/Texture/Effect/furit_12.png", 
                      L"Asset/Texture/Effect/furit_13.png", };

    t.MinPosition = { -80,50,-80 };
    t.MaxPosition = { 80,50, 80 };

    t.MaxParticles = 2000;
    t.EmitRate = 500.0f;


    t.MinVelocity = { -0.2f,-10.0f, -0.2f };
    t.MaxVelocity = { 0.2f, -15.0f,  0.2f };


    t.Acceleration = { 0.0f, -2.0f,  0.0f };


    t.MinLife = 3.5f;
    t.MaxLife = 3.6f;



    t.MinStartScale = 1;
    t.MaxStartScale = 1.8;

    t.MinEndScale = 1;
    t.MaxEndScale = 1.8;


    return t;
}

ParticleTemper ParticleTemperSettings::ShockWave()
{
    ParticleTemper t{};

    t.TexturePath =
    {
        L"Asset/Texture/Effect/ring_1.png",
        L"Asset/Texture/Effect/ring_2.png",
        L"Asset/Texture/Effect/ring_3.png",
    };


    t.MaxParticles = 1;
    t.EmitRate = 1;


    t.MinPosition = { 0,0,0 };
    t.MaxPosition = { 0,0,0 };


    t.MinVelocity = { 0,0,0 };
    t.MaxVelocity = { 0,0,0 };


    t.Acceleration = { 0,0,0 };


    t.MinLife = 3.0f;
    t.MaxLife = 3.0f;


    //ç≈èâè¨Ç≥Ç¢
    t.MinStartScale = 1.0f;
    t.MaxStartScale = 1.0f;


    //ç≈å„ãêëÂ
    t.MinEndScale = 50.0f;
    t.MaxEndScale = 50.0f;


    return t;
}