#include "sprite2DComponent.h"
#include "textureManager.h"
#include "resourceManager.h"
#include "texture.h"

void Sprite2DComponent::SetSprite(const wchar_t* path)
{
    m_Sprite = std::make_unique<Polygon2D>();
    m_Sprite->Init();
    auto tex = ResourceManager::Load<Texture>(path);
    m_Sprite->SetTexture(tex);
}
