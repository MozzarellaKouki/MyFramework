#include "sprite2DComponent.h"

void Sprite2DComponent::SetSprite(const wchar_t* path)
{
    m_Sprite = new Polygon2D();
    m_Sprite->Init();
    auto tex = TextureManager::Load(path);
    m_Sprite->SetTexture(tex);
}
