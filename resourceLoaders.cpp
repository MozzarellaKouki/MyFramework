#include "main.h"
#include "resourceManager.h"
#include "mesh.h"
#include "texture.h"
#include "modelLoader.h"
#include "renderer.h"

//--------------------------------------------------------------
// 型ごとの読み込み処理
//--------------------------------------------------------------

std::unique_ptr<Mesh> ResourceLoader<Mesh>::Load(const std::filesystem::path& path)
{
    //今は OBJ のみ対応
    return ModelLoader::LoadOBJ(path);
}

std::unique_ptr<Texture> ResourceLoader<Texture>::Load(const std::filesystem::path& path)
{
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage image;

    if (FAILED(DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, image)))
        return nullptr;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    if (FAILED(DirectX::CreateShaderResourceView(
        Renderer::GetDevice(),
        image.GetImages(),
        image.GetImageCount(),
        metadata,
        srv.GetAddressOf())))
    {
        return nullptr;
    }

    return std::make_unique<Texture>(std::move(srv));
}
