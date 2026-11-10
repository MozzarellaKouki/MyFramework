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

    //ÉGÉtÉFÉNÉgìoò^ÅEéÊìæ
    inline static void AddEffect(std::unique_ptr<PostEffect> effect)
    {
        if (!effect) return;

        //ìØÇ∂å^Ç™Ç∑Ç≈Ç…ìoò^Ç≥ÇÍÇƒÇ¢Ç»Ç¢Ç©ämîF
        for (auto& existing : m_Effects)
        {
            if (typeid(*existing) == typeid(*effect))
            {
                return;
            }
        }

        if (m_Initialized)
        {
            effect->Init();
        }

        m_Effects.push_back(std::move(effect));
    }


    template<typename T> inline static T* GetEffect()
    {
        for (auto& effect : m_Effects)
        {
            if (T* result = dynamic_cast<T*>(effect.get()))
            {
                return result;
            }
        }

        return nullptr;
    }



};
