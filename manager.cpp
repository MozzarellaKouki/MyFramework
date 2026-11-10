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



void Manager::Init()
{
    Renderer::Init();
    Renderer::SetPosterizeLevels(64.0f);

    if (!m_RenderTexture.Init(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        MessageBox(nullptr, "Scene render texture initialization failed.", "Error", MB_OK);
        return;
    }

    Audio::InitMaster();
    AudioManager::Init();
    SceneManager::Init(std::make_unique<TitleScene>());
    TextureManager::Init();

    //PostEffect Test
    PostEffectManager::AddEffect(std::make_unique<GrayScale>());
    PostEffectManager::AddEffect(std::make_unique<Bloom>());

    
    auto* grayscale = PostEffectManager::GetEffect<GrayScale>();
    grayscale->SetEnabled(true);
    grayscale->SetIntensity(0.0f);

    auto* bloom = PostEffectManager::GetEffect<Bloom>();
    bloom->SetEnabled(true);


  

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
    m_RenderTexture.Uninit();
    Renderer::Uninit();
}

void Manager::Update()
{
    SceneManager::Update();
    AudioManager::Update();
    PostEffectManager::Update();



    if (Keyboard_IsKeyDown(KK_P))
    {
        auto* grayscale = PostEffectManager::GetEffect<GrayScale>();
        grayscale->SetIntensity( std::min(grayscale->GetIntensity() + 0.01f, 1.0f));

    }

    if (Keyboard_IsKeyDown(KK_O))
    {
        auto* grayscale = PostEffectManager::GetEffect<GrayScale>();
        grayscale->SetIntensity(std::max(grayscale->GetIntensity() - 0.01f, 0.0f));
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
