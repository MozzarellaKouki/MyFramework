#include "main.h"
#include "GrayScale.h"
#include "renderer.h"

void GrayScale::Init()
{
    m_Display.Init("Shader//grayScalePS.cso");

    m_Display.SetPosition(0.0f, 0.0f);
    m_Display.SetSize( static_cast<float>(SCREEN_WIDTH),static_cast<float>(SCREEN_HEIGHT));
    m_Display.SetUV(0.0f, 0.0f, 1.0f, 1.0f);
    m_Display.SetBlendMode(BlendMode::Opaque);

    m_Initialized = true;
}

void GrayScale::Uninit()
{
    if (!m_Initialized) return;

    m_Display.Uninit();
    m_Initialized = false;
}

void GrayScale::Update()
{
}

void GrayScale::Draw( ID3D11ShaderResourceView* source, RenderTexture& destination)
{
    if (!m_Initialized || !source) return;


    Renderer::SetGrayscaleIntensity(m_Intensity);
    auto* context = Renderer::GetDeviceContext();
    ID3D11ShaderResourceView* nullSRV = nullptr;
    context->PSSetShaderResources(0, 1, &nullSRV);

    destination.SetRenderTarget();
    m_Display.SetTexture(source);

    m_Display.Draw();
    context->PSSetShaderResources(  0, 1, &nullSRV );
}
