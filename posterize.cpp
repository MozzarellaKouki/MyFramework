#include "main.h"
#include "posterize.h"
#include "renderer.h"

void Posterize::Init()
{
    if (m_Initialized) return;

    m_Display.Init("Shader//posterizePS.cso");
    m_Display.SetPosition(0.0f, 0.0f);
    m_Display.SetSize( static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));
    m_Display.SetUV(0.0f, 0.0f, 1.0f, 1.0f);
    m_Display.SetBlendMode(BlendMode::Opaque);

    m_Initialized = true;
}

void Posterize::Uninit()
{
    if (!m_Initialized) return;

    m_Display.Uninit();
    m_Initialized = false;
}

void Posterize::Draw(ID3D11ShaderResourceView* source, RenderTexture& destination)
{
    if (!m_Initialized || !source) return;

    Renderer::SetPosterizeLevels(m_Levels);

    auto* context = Renderer::GetDeviceContext();
    ID3D11ShaderResourceView* nullSRV = nullptr;
    context->PSSetShaderResources(0, 1, &nullSRV);

    destination.SetRenderTarget();
    m_Display.SetTexture(source);
    m_Display.SetColor(1.0f, 1.0f, 1.0f);
    m_Display.SetAlpha(1.0f);
    m_Display.Draw();

    context->PSSetShaderResources(0, 1, &nullSRV);
}

void Posterize::SetLevels(float levels)
{
    m_Levels = (levels < 2.0f) ? 2.0f : levels;
}

float Posterize::GetLevels() const
{
    return m_Levels;
}
