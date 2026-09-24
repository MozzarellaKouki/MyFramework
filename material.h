#pragma once
#include "main.h"

struct MaterialData
{
    XMFLOAT4 color = { 1,1,1,1 };
    ID3D11ShaderResourceView* texture = nullptr;
};