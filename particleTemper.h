#pragma once
#include "mathUtils.h"
#include "string"

struct ParticleTemper
{
    std::vector<std::wstring> TexturePath = { L"Asset/Texture/Effect/BlueDonut.png" };

    float MinLife = 1.0f;
    float MaxLife = 2.5f;

    float EmitRate = 10.0f;

    Vector3 MinPosition = { 0,0,0 };
    Vector3 MaxPosition = { 1,1,1 };

    Vector3 MinVelocity = { -2, 4, -2 };
    Vector3 MaxVelocity = { 2, 6, 2 };

    Vector3 Acceleration = { 0,-9.8f,0 };

    float MinStartScale = 1.0f;
    float MaxStartScale = 1.2f;
    float MinEndScale = 2.0f;
    float MaxEndScale = 2.5f;

    Color StartColor = {1,1,1,1};
    Color EndColor = { 1,1,1,1 };

    int MaxParticles = 1000;
};
