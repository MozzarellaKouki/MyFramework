#include "numberRenderer.h"
#include <string>
#include "renderer.h"
#include "textureManager.h"

void NumberRenderer::Init()
{
    m_NumberSprite.Init();
    m_NumberSprite.SetTexture(TextureManager::Load(L"Asset/Texture/NumberSpriteSheet.png"));
}


void NumberRenderer::Uninit()
{
    m_NumberSprite.Uninit();
}


void NumberRenderer::Draw(int number, float x, float y)
{
    std::string str = std::to_string(number);

    const float size = 32.0f;

    for (int i = 0; i < str.size(); i++)
    {
        int digit = str[i] - '0';

        SPRITE_ANIMATION anim{};
        anim.Frame = digit;
        anim.Columns = 5;
        anim.Rows = 5;

        Renderer::SetSpriteAnimation(anim);

        m_NumberSprite.SetPosition
        (
            x + i * size,
            y
        );

        m_NumberSprite.SetSize
        (
            size,
            size
        );

        m_NumberSprite.Draw();
    }
}