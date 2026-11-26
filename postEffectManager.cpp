#include "main.h"
#include "postEffectManager.h"
#include "renderer.h"

std::vector<std::unique_ptr<PostEffect>> PostEffectManager::m_Effects;

RenderTexture PostEffectManager::m_Ping;
RenderTexture PostEffectManager::m_Pong;

FullscreenQuad PostEffectManager::m_Display;
bool PostEffectManager::m_Initialized = false;
bool PostEffectManager::m_DisplayInitialized = false;

void PostEffectManager::Init()
{
    if (m_Initialized) return;

    if (!m_DisplayInitialized)
    {
        m_Display.Init();
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

    for (auto& effect : m_Effects)
    {
        effect->Init();
    }

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

        //実際に適用した回数で Ping / Pong を切り替える
        RenderTexture& target = (appliedCount % 2 == 0) ? m_Ping : m_Pong;

        //シーン描画など、ポストエフェクトの外で設定されたテクスチャが残っていても大丈夫なように空にしておく
        //（エフェクト内のパス同士は FullscreenQuad::Draw が後片付けする）
        ID3D11ShaderResourceView* nullSRV = nullptr;
        context->PSSetShaderResources(0, 1, &nullSRV);

        effect->Draw(currentSource, target);
        currentSource = target.GetShaderResourceView();
        appliedCount++;
    }


    Renderer::SetBackBufferRenderTarget();
    m_Display.Draw(currentSource);
}




