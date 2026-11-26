#pragma once
#include "mathUtils.h"
#include "resourceHandle.h"
#include "texture.h"


struct Particle
{
    bool Alive = false;

    Handle<Texture> Texture;

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
