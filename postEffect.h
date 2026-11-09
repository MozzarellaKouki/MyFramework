#pragma once

#include <d3d11.h>
#include "renderTexture.h"

class PostEffect
{
protected:
    bool m_Enabled = false;
    float m_Intensity = 0.0f;

public:
    virtual ~PostEffect() = default;

    virtual void Init() = 0;
    virtual void Uninit() = 0;
    virtual void Draw(ID3D11ShaderResourceView* source, RenderTexture& destination) = 0;
    virtual void Update() {}

    //利用可能フラグ
    void SetEnabled(bool enabled) { m_Enabled = enabled; }
    bool IsEnabled() const { return m_Enabled; }

    //強度
    void SetIntensity(float intensity) { m_Intensity = intensity; }
    float GetIntensity() const { return m_Intensity; }

};
