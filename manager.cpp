#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "sceneManager.h"
#include "titleScene.h"
#include "audioManager.h"
#include "audio.h"
#include "keyboard.h"
#include "resourceManager.h"
#include "postEffectManager.h"
#include "bloom.h"
#include "grayScale.h"
#include "posterize.h"
#include "debugCommands.h"
#include "collisionManager.h"
#include <memory>



void Manager::Init()
{
    Renderer::Init();

    if (!m_RenderTexture.Init(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        MessageBox(nullptr, "Scene render texture initialization failed.", "Error", MB_OK);
        return;
    }

    Audio::InitMaster();
    AudioManager::Init();
    SceneManager::Init(std::make_unique<TitleScene>());
  

    //PostEffect Test
    PostEffectManager::AddEffect(std::make_unique<GrayScale>());
    PostEffectManager::AddEffect(std::make_unique<Bloom>());
    PostEffectManager::AddEffect(std::make_unique<Posterize>());

    
    auto* grayscale = PostEffectManager::GetEffect<GrayScale>();
    grayscale->SetEnabled(true);
    grayscale->SetIntensity(0.0f);

    auto* bloom = PostEffectManager::GetEffect<Bloom>();
    bloom->SetEnabled(true);

    auto* posterize = PostEffectManager::GetEffect<Posterize>();
    posterize->SetEnabled(false);
    posterize->SetLevels(3);


  

    // エフェクトの初期化
    PostEffectManager::Init();

    RegisterDebugCommands();

}

void Manager::Uninit()
{
    SceneManager::Uninit();
    AudioManager::Uninit();
    Audio::UninitMaster();
    ResourceManager::ReleaseAll(); //Renderer（デバイス）より先に解放する

    PostEffectManager::Uninit();
    m_RenderTexture.Uninit();
    Renderer::Uninit();
}

void Manager::Update()
{
    SceneManager::Update();
    AudioManager::Update();
    PostEffectManager::Update();

    DebugCommands::Update();
}

void Manager::RegisterDebugCommands()
{
    DebugCommands::AddHold(KK_P, "グレースケールを強くする", []
    {
        auto* grayscale = PostEffectManager::GetEffect<GrayScale>();
        grayscale->SetIntensity(std::min(grayscale->GetIntensity() + 0.01f, 1.0f));
    }, DebugScope::Global);

    DebugCommands::AddHold(KK_O, "グレースケールを弱くする", []
    {
        auto* grayscale = PostEffectManager::GetEffect<GrayScale>();
        grayscale->SetIntensity(std::max(grayscale->GetIntensity() - 0.01f, 0.0f));
    }, DebugScope::Global);

    DebugCommands::AddTrigger(KK_F2, "当たり判定の状態を表示", []
    {
        CollisionManager::PrintDebugInfo();
    }, DebugScope::Global);

    DebugCommands::AddTrigger(KK_F3, "読み込み中のリソースを表示", []
    {
        ResourceManager::PrintDebugInfo();
    }, DebugScope::Global);
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
