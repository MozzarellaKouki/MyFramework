#pragma once
#include "mathUtils.h"


struct Particle
{
    bool Alive = false;

    ID3D11ShaderResourceView* Texture = nullptr;

    Vector3 Position;
    Vector3 Velocity;
    Vector3 Acceleration;

    float Rotation;
    float RotationSpeed;

    float Life;
    float LifeLimit;

    float Scale;
    float StartScale;
    float EndScale;
    float MinStartScale;
    float MaxStartScale;
    float MinEndScale;
    float MaxEndScale;
  
};
