#pragma once

#include <d3d11.h>
#include "renderTexture.h"

class PostEffect
{
public:
    virtual ~PostEffect() = default;

    virtual void Init() = 0;
    virtual void Uninit() = 0;
    virtual void Draw(
        ID3D11ShaderResourceView* source,
        RenderTexture& destination) = 0;
    virtual void Update() {}
};