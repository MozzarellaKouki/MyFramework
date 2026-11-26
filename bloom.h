#pragma once

#include "PostEffect.h"
#include "RenderTexture.h"
#include "fullscreenQuad.h"

class Bloom : public PostEffect
{
private:
    FullscreenQuad m_Display;
    RenderTexture m_ExtractTexture;
    FullscreenQuad m_ExtractQuad;
    RenderTexture m_BlurHorizontalTexture;
    FullscreenQuad m_BlurHorizontalQuad;
    RenderTexture m_BlurVerticalTexture;
    FullscreenQuad m_BlurVerticalQuad;


public:
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw(ID3D11ShaderResourceView* source, RenderTexture& destination) override;


};