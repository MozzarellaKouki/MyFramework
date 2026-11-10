#pragma once

#include "PostEffect.h"
#include "Polygon2D.h"

class Posterize : public PostEffect
{
private:
    Polygon2D m_Display;
    float m_Levels = 16.0f;

public:
    void Init() override;
    void Uninit() override;
    void Draw( ID3D11ShaderResourceView* source, RenderTexture& destination) override;

    void SetLevels(float levels);
    float GetLevels() const;
};
