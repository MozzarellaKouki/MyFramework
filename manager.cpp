#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "sceneManager.h"
#include "titleScene.h"
#include "audioManager.h"
#include "audio.h"
#include "keyboard.h"
#include "textureManager.h"
#include "postEffectManager.h"
#include "bloom.h"
#include "grayScale.h"
#include <memory>

RenderTexture Manager::m_RenderTexture;
Bloom* Manager::m_Bloom = nullptr;
GrayScale* Manager::m_GrayScale = nullptr;




void Manager::Init()
{
    Renderer::Init();
    Renderer::SetPosterizeLevels(16.0f);

    if (!m_RenderTexture.Init(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        MessageBox(nullptr, "Scene render texture initialization failed.",
            "Error", MB_OK);
        return;
    }

    Audio::InitMaster();
    AudioManager::Init();
    SceneManager::Init(std::make_unique<TitleScene>());
    TextureManager::Init();


    // Grayscale
    auto grayscale = std::make_unique<GrayScale>();
    m_GrayScale = grayscale.get();
    m_GrayScale->SetIntensity(0.5f);
    m_GrayScale->SetEnabled(true);

    // Bloom
    auto bloom = std::make_unique<Bloom>();
    m_Bloom = bloom.get();
    m_Bloom->SetIntensity(0.5f);
    m_Bloom->SetEnabled(true);


    // PostEffectManagerへ登録
    PostEffectManager::AddEffect(std::make_unique<Bloom>());
    PostEffectManager::AddEffect(std::make_unique<GrayScale>());

    // エフェクトの初期化
    PostEffectManager::Init();


}

void Manager::Uninit()
{
    SceneManager::Uninit();
    AudioManager::Uninit();
    Audio::UninitMaster();
    TextureManager::Uninit();

    PostEffectManager::Uninit();
    m_Bloom = nullptr;
    m_GrayScale = nullptr;
    m_RenderTexture.Uninit();
    Renderer::Uninit();
}

void Manager::Update()
{
    SceneManager::Update();
    AudioManager::Update();
    PostEffectManager::Update();


    //デバッグ
    if (!m_Bloom) return;

    float intensity = m_Bloom->GetIntensity();
    if (Keyboard_IsKeyDownTrigger(KK_P))
    {
        intensity += 0.1f;
        if (intensity > 2.0f)
            intensity = 2.0f;
        m_Bloom->SetIntensity(intensity);
    }

    if (Keyboard_IsKeyDownTrigger(KK_O))
    {
        intensity -= 0.1f;
        if (intensity < 0.0f)
            intensity = 0.0f;
        m_Bloom->SetIntensity(intensity);
    }
}

void Manager::Draw()
{
    Renderer::Begin();

    auto* context = Renderer::GetDeviceContext();
    ID3D11ShaderResourceView* nullSRV = nullptr;
    context->PSSetShaderResources(0, 1, &nullSRV);

    m_RenderTexture.SetRenderTarget();
    SceneManager::DrawScene();

    SPRITE_ANIMATION animation{};
    animation.Frame = 0;
    animation.Columns = 1;
    animation.Rows = 1;
    Renderer::SetSpriteAnimation(animation);

    PostEffectManager::Draw(m_RenderTexture.GetShaderResourceView());
    SceneManager::DrawFade();
    Renderer::End();
}
