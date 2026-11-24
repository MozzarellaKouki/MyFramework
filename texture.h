#pragma once
#include <d3d11.h>
#include <wrl/client.h>

//--------------------------------------------------------------
// Texture
//
// シェーダーリソースビューを持つだけのリソース。
// ComPtr が持っているので、Texture が破棄されると自動で Release される。
//--------------------------------------------------------------
class Texture
{
public:

    explicit Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
        : m_SRV(std::move(srv)) {}

    ID3D11ShaderResourceView* GetSRV() const { return m_SRV.Get(); }

private:

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
};
