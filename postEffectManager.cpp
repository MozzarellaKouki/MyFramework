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
    if (m_Initialized) return;

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
        MessageBox(nullptr, "PostEffect RT Initialization failed.","Error", MB_OK);
        m_Ping.Uninit();
        m_Pong.Uninit();
        return;
    }

    for(auto& effect : m_Effects) effect->Init();

    m_Initialized = true;
}

void PostEffectManager::Uninit()
{
    if (m_Initialized)
    {
        for(auto& effect : m_Effects) effect->Uninit();
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
    for(auto& effect : m_Effects) effect->Update();
}


void PostEffectManager::Draw(ID3D11ShaderResourceView* source)
{
    if (!source) return;
    if (!m_DisplayInitialized) return;


    ID3D11ShaderResourceView* currentSource = source;
    auto* context = Renderer::GetDeviceContext();

    size_t appliedCount = 0;

    for (auto& effect : m_Effects)
    {
        if (!effect->IsEnabled()) continue;

        //ŽÀÛ‚É“K—p‚µ‚½‰ñ”‚Å Ping / Pong ‚ðØ‚è‘Ö‚¦‚é
        RenderTexture& target = (appliedCount % 2 == 0) ? m_Ping : m_Pong;

        ID3D11ShaderResourceView* nullSRV = nullptr;
        context->PSSetShaderResources(0, 1, &nullSRV);

        effect->Draw(currentSource, target);
        currentSource = target.GetShaderResourceView();
        appliedCount++;
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


PostEffectHandle PostEffectManager::AddEffect(std::unique_ptr<PostEffect> effect)
{
    if (!effect) return static_cast<size_t>(-1);

    if (m_Initialized) effect->Init();
    m_Effects.push_back(std::move(effect));

    return m_Effects.size() - 1;
}


void PostEffectManager::SetEnabled(PostEffectHandle handle, bool enabled)
{
    if (handle >= m_Effects.size()) return;
    m_Effects[handle]->SetEnabled(enabled);
}

bool PostEffectManager::IsEnabled(PostEffectHandle handle)
{
    if (handle >= m_Effects.size()) return false;
    return m_Effects[handle]->IsEnabled();
}

void PostEffectManager::SetIntensity(PostEffectHandle handle, float intensity)
{
    if (handle >= m_Effects.size()) return;
    m_Effects[handle]->SetIntensity(intensity);
}

float PostEffectManager::GetIntensity(PostEffectHandle handle)
{
    if (handle >= m_Effects.size()) return 0.0f;
    return m_Effects[handle]->GetIntensity();
}
