#pragma once

#include "PostEffect.h"
#include "Polygon2D.h"

class GrayScale : public PostEffect
{
private:
    Polygon2D m_Display;
    float m_Intensity = 0.5f;
    bool m_Initialized = false;

public:
    void Init() override;
    void Uninit() override;
    void Update() override;

    void Draw(ID3D11ShaderResourceView* source, RenderTexture& destination) override;


};
