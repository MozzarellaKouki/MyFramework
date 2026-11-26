#pragma once
#include "main.h"
#include "resourceHandle.h"
#include "texture.h"

struct MaterialData
{
    DirectX::XMFLOAT4 color = { 1,1,1,1 };
    Handle<Texture> texture;
};