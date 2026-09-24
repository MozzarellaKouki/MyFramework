#include "main.h"
#include "TextureManager.h"
#include "renderer.h"


std::map<std::wstring, ID3D11ShaderResourceView*> TextureManager::m_TextureMap;

void TextureManager::Init()
{
    m_TextureMap.clear();
}

void TextureManager::Uninit()
{
    for (auto& tex : m_TextureMap)
    {
        if (tex.second)
        {
            tex.second->Release();
        }
    }
    m_TextureMap.clear();
}

ID3D11ShaderResourceView* TextureManager::Load(const std::wstring& fileName)
{
    auto it = m_TextureMap.find(fileName);
    if (it != m_TextureMap.end())
        return it->second;

    ID3D11ShaderResourceView* texture = nullptr;


    TexMetadata metadata;
    ScratchImage image;

    LoadFromWICFile(
        fileName.c_str(),
        WIC_FLAGS_NONE,
        &metadata,
        image
    );

    CreateShaderResourceView(
        Renderer::GetDevice(),
        image.GetImages(),
        image.GetImageCount(),
        metadata,
        &texture
    );

    assert(texture);
    m_TextureMap[fileName] = texture;


    return texture;
}