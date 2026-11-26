#include "fullscreenQuad.h"

void FullscreenQuad::Init(const char* pixelShaderFile)
{
    //画面全体を覆う四角形（2D の座標系はピクセル単位、左上が原点）
    const float w = static_cast<float>(SCREEN_WIDTH);
    const float h = static_cast<float>(SCREEN_HEIGHT);

    VERTEX_3D vertices[4]{};
    vertices[0].Position = { 0.0f, 0.0f, 0.0f }; vertices[0].TexCoord = { 0.0f, 0.0f };
    vertices[1].Position = { w,    0.0f, 0.0f }; vertices[1].TexCoord = { 1.0f, 0.0f };
    vertices[2].Position = { 0.0f, h,    0.0f }; vertices[2].TexCoord = { 0.0f, 1.0f };
    vertices[3].Position = { w,    h,    0.0f }; vertices[3].TexCoord = { 1.0f, 1.0f };

    for (auto& v : vertices)
    {
        v.Normal = { 0.0f, 0.0f, 0.0f };
        v.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
    }

    //頂点は変わらないので、書き換えない設定（IMMUTABLE）で作る
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertices;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "Shader//unlitTextureVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader, pixelShaderFile);
}


void FullscreenQuad::Uninit()
{
    if (m_PixelShader)  { m_PixelShader->Release();  m_PixelShader = nullptr; }
    if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
    if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }
    if (m_VertexBuffer) { m_VertexBuffer->Release(); m_VertexBuffer = nullptr; }
}


void FullscreenQuad::Draw(ID3D11ShaderResourceView* source, BlendMode blend,
                          const DirectX::XMFLOAT4& color)
{
    if (!source || !m_VertexBuffer)
        return;

    auto* context = Renderer::GetDeviceContext();

    //シェーダー
    context->IASetInputLayout(m_VertexLayout);
    context->VSSetShader(m_VertexShader, nullptr, 0);
    context->PSSetShader(m_PixelShader, nullptr, 0);

    //行列（頂点が画面座標なので、ワールドは単位行列）
    Renderer::SetWorldViewProjection2D();
    Renderer::SetWorldMatrix(DirectX::XMMatrixIdentity());
    Renderer::SetBlendMode(blend);

    //マテリアル
    MATERIAL material{};
    material.Diffuse = color;
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    //テクスチャ（描いている間だけ借りる）
    context->PSSetShaderResources(0, 1, &source);

    //描画
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    context->Draw(4, 0);

    //借りたテクスチャを返す（スロット0を空にする）
    ID3D11ShaderResourceView* nullSRV = nullptr;
    context->PSSetShaderResources(0, 1, &nullSRV);
}
