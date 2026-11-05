#include "main.h"
#include "bloom.h"
#include "renderer.h"

void Bloom::Init()
{
    if (!m_ExtractTexture.Init(SCREEN_WIDTH, SCREEN_HEIGHT) ||
        !m_BlurHorizontalTexture.Init(SCREEN_WIDTH, SCREEN_HEIGHT) ||
        !m_BlurVerticalTexture.Init(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        MessageBox(nullptr, "Bloom render target initialization failed.",
            "Error", MB_OK);
        m_ExtractTexture.Uninit();
        m_BlurHorizontalTexture.Uninit();
        m_BlurVerticalTexture.Uninit();
        return;
    }

    m_Display.Init();
    m_Display.SetPosition(0.0f, 0.0f);
    m_Display.SetSize(static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT));
    m_Display.SetUV(0.0f, 0.0f, 1.0f, 1.0f);

    m_ExtractQuad.Init("Shader//bloomExtractPS.cso");
    m_ExtractQuad.SetPosition(0.0f, 0.0f);
    m_ExtractQuad.SetSize(static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT));
    m_ExtractQuad.SetUV(0.0f, 0.0f, 1.0f, 1.0f);
    m_ExtractQuad.SetBlendMode(BlendMode::Opaque);

    m_BlurHorizontalQuad.Init("Shader//bloomBlurHorizontalPS.cso");
    m_BlurHorizontalQuad.SetPosition(0.0f, 0.0f);
    m_BlurHorizontalQuad.SetSize(static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT));
    m_BlurHorizontalQuad.SetUV(0.0f, 0.0f, 1.0f, 1.0f);
    m_BlurHorizontalQuad.SetBlendMode(BlendMode::Opaque);

    m_BlurVerticalQuad.Init("Shader//bloomBlurVerticalPS.cso");
    m_BlurVerticalQuad.SetPosition(0.0f, 0.0f);
    m_BlurVerticalQuad.SetSize(static_cast<float>(SCREEN_WIDTH),
        static_cast<float>(SCREEN_HEIGHT));
    m_BlurVerticalQuad.SetUV(0.0f, 0.0f, 1.0f, 1.0f);
    m_BlurVerticalQuad.SetBlendMode(BlendMode::Opaque);

    m_Initialized = true;
}

void Bloom::Uninit()
{
    if (m_Initialized)
    {
        m_BlurVerticalQuad.Uninit();
        m_BlurHorizontalQuad.Uninit();
        m_ExtractQuad.Uninit();
        m_Display.Uninit();
        m_Initialized = false;
    }

    m_BlurVerticalTexture.Uninit();
    m_BlurHorizontalTexture.Uninit();
    m_ExtractTexture.Uninit();
}

void Bloom::Update()
{
}

void Bloom::Draw(
    ID3D11ShaderResourceView* source,
    RenderTexture& destination)
{
    if (!m_Initialized || !source)
        return;

    auto* context = Renderer::GetDeviceContext();
    ID3D11ShaderResourceView* nullSRV = nullptr;
    SPRITE_ANIMATION animation{};
    animation.Frame = 0;
    animation.Columns = 1;
    animation.Rows = 1;
    Renderer::SetSpriteAnimation(animation);

    context->PSSetShaderResources(0, 1, &nullSRV);
    m_ExtractTexture.SetRenderTarget();
    m_ExtractQuad.SetTexture(source);
    m_ExtractQuad.Draw();

    context->PSSetShaderResources(0, 1, &nullSRV);
    m_BlurHorizontalTexture.SetRenderTarget();
    m_BlurHorizontalQuad.SetTexture(
        m_ExtractTexture.GetShaderResourceView());
    m_BlurHorizontalQuad.Draw();

    context->PSSetShaderResources(0, 1, &nullSRV);
    m_BlurVerticalTexture.SetRenderTarget();
    m_BlurVerticalQuad.SetTexture(
        m_BlurHorizontalTexture.GetShaderResourceView());
    m_BlurVerticalQuad.Draw();

    context->PSSetShaderResources(0, 1, &nullSRV);
    destination.SetRenderTarget();

    m_Display.SetTexture(source);
    m_Display.SetBlendMode(BlendMode::Opaque);
    m_Display.SetColor(1.0f, 1.0f, 1.0f);
    m_Display.SetAlpha(1.0f);
    m_Display.Draw();

    m_Display.SetTexture(m_BlurVerticalTexture.GetShaderResourceView());
    m_Display.SetBlendMode(BlendMode::Additive);
    m_Display.SetColor(m_Intensity, m_Intensity, m_Intensity);
    m_Display.Draw();

    context->PSSetShaderResources(0, 1, &nullSRV);
}

void Bloom::SetIntensity(float intensity)
{
    m_Intensity = intensity;
}

float Bloom::GetIntensity() const
{
    return m_Intensity;
}