#pragma once

#include <memory>
#include <vector>
#include "PostEffect.h"
#include "RenderTexture.h"
#include "Polygon2D.h"

using PostEffectHandle = size_t;

class PostEffectManager
{
private:
    static std::vector<std::unique_ptr<PostEffect>> m_Effects;
    static RenderTexture m_Ping;
    static RenderTexture m_Pong;
    static Polygon2D m_Display;
    static bool m_Initialized;
    static bool m_DisplayInitialized;

    bool m_Enabled = false;

public:
    static void Init();
    static void Uninit();
    static void Update();
    static void Draw(ID3D11ShaderResourceView* source);

    //エフェクト登録
    static PostEffectHandle AddEffect(std::unique_ptr<PostEffect> effect);

    //仕様フラグ
    static void SetEnabled(PostEffectHandle handle, bool enabled);
    static bool IsEnabled(PostEffectHandle handle);

    //強度
    static void SetIntensity(PostEffectHandle handle, float intensity);
    static float GetIntensity(PostEffectHandle handle);
};
