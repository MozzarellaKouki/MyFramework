
#include "main.h"
#include "renderer.h"
#include "RenderTexture.h"

bool RenderTexture::Init(int width, int height)
{
    m_Width = width;
    m_Height = height;

    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = static_cast<UINT>(width);
    desc.Height = static_cast<UINT>(height);
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags =
        D3D11_BIND_RENDER_TARGET |
        D3D11_BIND_SHADER_RESOURCE;

    ID3D11Device* device = Renderer::GetDevice();

    HRESULT hr = device->CreateTexture2D(
        &desc, nullptr, &m_Texture);

    if (FAILED(hr))
        return false;

    hr = device->CreateRenderTargetView(
        m_Texture, nullptr, &m_RenderTargetView);

    if (FAILED(hr))
    {
        Uninit();
        return false;
    }

    // 深度バッファ用テクスチャを作成
    D3D11_TEXTURE2D_DESC depthDesc{};
    depthDesc.Width = static_cast<UINT>(m_Width);
    depthDesc.Height = static_cast<UINT>(m_Height);
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D16_UNORM;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;


    hr = device->CreateTexture2D
    (
        &depthDesc,
        nullptr,
        &m_DepthTexture
    );

    if (FAILED(hr))
    {
        Uninit();
        return false;
    }


    hr = device->CreateDepthStencilView
    (
        m_DepthTexture,
        nullptr,
        &m_DepthStencilView
    );

    if (FAILED(hr))
    {
        Uninit();
        return false;
    }

    hr = device->CreateShaderResourceView( m_Texture, nullptr, &m_ShaderResourceView);

    if (FAILED(hr))
    {
        Uninit();
        return false;
    }

    return true;
}

void RenderTexture::Uninit()
{
    if (m_ShaderResourceView)
    {
        m_ShaderResourceView->Release();
        m_ShaderResourceView = nullptr;
    }

    if (m_DepthStencilView)
    {
        m_DepthStencilView->Release();
        m_DepthStencilView = nullptr;
    }

    if (m_DepthTexture)
    {
        m_DepthTexture->Release();
        m_DepthTexture = nullptr;
    }

    if (m_RenderTargetView)
    {
        m_RenderTargetView->Release();
        m_RenderTargetView = nullptr;
    }

    if (m_Texture)
    {
        m_Texture->Release();
        m_Texture = nullptr;
    }

    m_Width = 0;
    m_Height = 0;
}

void RenderTexture::SetRenderTarget()
{
    ID3D11DeviceContext* context = Renderer::GetDeviceContext();

    // RenderTextureを描画先に設定
    context->OMSetRenderTargets(
        1,
        &m_RenderTargetView,
        m_DepthStencilView
    );

    // ビューポートを設定
    D3D11_VIEWPORT viewport{};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(m_Width);
    viewport.Height = static_cast<float>(m_Height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);

    // RenderTextureをクリア
    float clearColor[4] = {
        0.2196f, 0.9137f, 0.5216f, 1.0f
    };

    context->ClearRenderTargetView(
        m_RenderTargetView,
        clearColor
    );

    // RenderTexture用の深度バッファをクリア
    context->ClearDepthStencilView(
        m_DepthStencilView,
        D3D11_CLEAR_DEPTH,
        1.0f,
        0
    );
}