#include "main.h"
#include "postEffectManager.h"
#include "renderer.h"

std::vector<std::unique_ptr<PostEffect>> PostEffectManager::m_Effects;

RenderTexture PostEffectManager::m_Ping;
RenderTexture PostEffectManager::m_Pong;

Polygon2D PostEffectManager::m_Display;
bool PostEffectManager::m_Initialized = false;
bool PostEffectManager::m_DisplayInitialized = false;

void PostEffectManager::Init()
{
    if (m_Initialized)
        return;

    if (!m_DisplayInitialized)
    {
        m_Display.Init();
        m_Display.SetPosition(0.0f, 0.0f);
        m_Display.SetSize(static_cast<float>(SCREEN_WIDTH),
            static_cast<float>(SCREEN_HEIGHT));
        m_Display.SetUV(0.0f, 0.0f, 1.0f, 1.0f);
        m_Display.SetBlendMode(BlendMode::Opaque);
        m_DisplayInitialized = true;
    }

    if (!m_Ping.Init(SCREEN_WIDTH, SCREEN_HEIGHT) ||
        !m_Pong.Init(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        MessageBox(nullptr, "Post effect render target initialization failed.",
            "Error", MB_OK);
        m_Ping.Uninit();
        m_Pong.Uninit();
        return;
    }

    for (auto& effect : m_Effects)
        effect->Init();

    m_Initialized = true;
}

void PostEffectManager::Uninit()
{
    if (m_Initialized)
    {
        for (auto& effect : m_Effects)
            effect->Uninit();
    }
    m_Effects.clear();

    if (m_DisplayInitialized)
    {
        m_Display.Uninit();
        m_DisplayInitialized = false;
    }
    m_Pong.Uninit();
    m_Ping.Uninit();
    m_Initialized = false;
}

void PostEffectManager::Update()
{
    for (auto& effect : m_Effects)
        effect->Update();
}

void PostEffectManager::Draw(ID3D11ShaderResourceView* source)
{
    if (!source) return;
    if (!m_DisplayInitialized) return;

    ID3D11ShaderResourceView* currentSource = source;
    auto* context = Renderer::GetDeviceContext();

    for (size_t i = 0; m_Initialized && i < m_Effects.size(); i++)
    {
        RenderTexture& target = (i % 2 == 0) ? m_Ping : m_Pong;

        ID3D11ShaderResourceView* nullSRV = nullptr;
        context->PSSetShaderResources(0, 1, &nullSRV);

        m_Effects[i]->Draw(currentSource, target);
        currentSource = target.GetShaderResourceView();
    }

    Renderer::SetBackBufferRenderTarget();
    m_Display.SetTexture(currentSource);
    m_Display.SetBlendMode(BlendMode::Opaque);
    m_Display.SetColor(1.0f, 1.0f, 1.0f);
    m_Display.SetAlpha(1.0f);
    m_Display.Draw();

    ID3D11ShaderResourceView* nullSRV = nullptr;
    context->PSSetShaderResources(0, 1, &nullSRV);
}

void PostEffectManager::AddEffect(std::unique_ptr<PostEffect> effect)
{
    if (!effect) return;
    if (m_Initialized) effect->Init();

    m_Effects.push_back(std::move(effect));
}
