#pragma once

#include "PostEffect.h"
#include "fullscreenQuad.h"

class GrayScale : public PostEffect
{
private:
    FullscreenQuad m_Display;

public:
    void Init() override;
    void Uninit() override;
    void Update() override;

    void Draw(ID3D11ShaderResourceView* source, RenderTexture& destination) override;


};
