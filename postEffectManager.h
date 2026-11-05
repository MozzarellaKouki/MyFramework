#pragma once

#include <memory>
#include <vector>
#include "PostEffect.h"
#include "RenderTexture.h"
#include "Polygon2D.h"

class PostEffectManager
{
private:
    static std::vector<std::unique_ptr<PostEffect>> m_Effects;
    static RenderTexture m_Ping;
    static RenderTexture m_Pong;
    static Polygon2D m_Display;
    static bool m_Initialized;
    static bool m_DisplayInitialized;

public:
    static void Init();
    static void Uninit();
    static void Update();
    static void Draw(ID3D11ShaderResourceView* source);

    static void AddEffect(std::unique_ptr<PostEffect> effect);
};
