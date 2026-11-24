#include "sprite2DComponent.h"
#include "textureManager.h"

void Sprite2DComponent::SetSprite(const wchar_t* path)
{
    m_Sprite = std::make_unique<Polygon2D>();
    m_Sprite->Init();
    auto tex = TextureManager::Load(path);
    m_Sprite->SetTexture(tex);
}
