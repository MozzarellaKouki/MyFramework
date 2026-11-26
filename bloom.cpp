#include "main.h"
#include "bloom.h"
#include "renderer.h"

void Bloom::Init()
{
    if (!m_ExtractTexture.Init(SCREEN_WIDTH, SCREEN_HEIGHT) ||
        !m_BlurHorizontalTexture.Init(SCREEN_WIDTH, SCREEN_HEIGHT) ||
        !m_BlurVerticalTexture.Init(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        MessageBox(nullptr, "Bloom render target initialization failed.", "Error", MB_OK);
        m_ExtractTexture.Uninit();
        m_BlurHorizontalTexture.Uninit();
        m_BlurVerticalTexture.Uninit();
        return;
    }

    m_Display.Init();
    m_ExtractQuad.Init("Shader//bloomExtractPS.cso");
    m_BlurHorizontalQuad.Init("Shader//bloomBlurHorizontalPS.cso");
    m_BlurVerticalQuad.Init("Shader//bloomBlurVerticalPS.cso");

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

void Bloom::Draw(ID3D11ShaderResourceView* source, RenderTexture& destination)
{
    if (!m_Initialized || !source) return;

    SPRITE_ANIMATION animation{};
    animation.Frame = 0;
    animation.Columns = 1;
    animation.Rows = 1;
    Renderer::SetSpriteAnimation(animation);

    //1. 明るい部分を抜き出す
    m_ExtractTexture.SetRenderTarget();
    m_ExtractQuad.Draw(source);

    //2. 横にぼかす
    m_BlurHorizontalTexture.SetRenderTarget();
    m_BlurHorizontalQuad.Draw(m_ExtractTexture.GetShaderResourceView());

    //3. 縦にぼかす
    m_BlurVerticalTexture.SetRenderTarget();
    m_BlurVerticalQuad.Draw(m_BlurHorizontalTexture.GetShaderResourceView());

    //4. 元の画像に、ぼかした光を加算する
    destination.SetRenderTarget();
    m_Display.Draw(source);
    m_Display.Draw(m_BlurVerticalTexture.GetShaderResourceView(), BlendMode::Additive,
        { m_Intensity, m_Intensity, m_Intensity, 1.0f });
}
