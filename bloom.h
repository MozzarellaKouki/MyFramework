#pragma once

#include "PostEffect.h"
#include "RenderTexture.h"
#include "Polygon2D.h"

class Bloom : public PostEffect
{
private:
    Polygon2D m_Display;
    RenderTexture m_ExtractTexture;
    Polygon2D m_ExtractQuad;
    RenderTexture m_BlurHorizontalTexture;
    Polygon2D m_BlurHorizontalQuad;
    RenderTexture m_BlurVerticalTexture;
    Polygon2D m_BlurVerticalQuad;


public:
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw(ID3D11ShaderResourceView* source, RenderTexture& destination) override;


};