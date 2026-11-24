#include "main.h"
#include "textureManager.h"
#include "resourceManager.h"
#include "texture.h"

ID3D11ShaderResourceView* TextureManager::Load(const std::wstring& fileName)
{
    const Handle<Texture> handle = ResourceManager::Load<Texture>(fileName, ResourceScope::Global);

    Texture* texture = ResourceManager::Get(handle);
    assert(texture && "テクスチャの読み込みに失敗（出力ウィンドウ参照）");

    return texture ? texture->GetSRV() : nullptr;
}
