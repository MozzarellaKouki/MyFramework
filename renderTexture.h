
#pragma once

#include <d3d11.h>

class RenderTexture
{
private:
    ID3D11Texture2D* m_Texture = nullptr;
    ID3D11RenderTargetView* m_RenderTargetView = nullptr;
    ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;
    ID3D11Texture2D* m_DepthTexture = nullptr;
    ID3D11DepthStencilView* m_DepthStencilView = nullptr;

    int m_Width = 0;
    int m_Height = 0;

public:
    bool Init(int width, int height);
    void Uninit();

    void SetRenderTarget();

    ID3D11ShaderResourceView* GetShaderResourceView() const
    {
        return m_ShaderResourceView;
    }
};
