#include "main.h"
#include "renderer.h"
#include "textureManager.h"
#include "polygon3D.h"



void Polygon3D::Init()
{

	m_Vertices[0].Position = { -0.5f,  0.5f, 0.0f };
	m_Vertices[0].Normal = XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	m_Vertices[0].Diffuse = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
	m_Vertices[0].TexCoord = XMFLOAT2{ 0.0f, 0.0f };

	m_Vertices[1].Position = { 0.5f,  0.5f, 0.0f };
	m_Vertices[1].Normal = XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	m_Vertices[1].Diffuse = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
	m_Vertices[1].TexCoord = XMFLOAT2{ 1.0f, 0.0f };

	m_Vertices[2].Position = { -0.5f, -0.5f, 0.0f };
	m_Vertices[2].Normal = XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	m_Vertices[2].Diffuse = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
	m_Vertices[2].TexCoord = XMFLOAT2{ 0.0f, 1.0f };

	m_Vertices[3].Position = { 0.5f, -0.5f, 0.0f };
	m_Vertices[3].Normal = XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	m_Vertices[3].Diffuse = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
	m_Vertices[3].TexCoord = XMFLOAT2{ 1.0f, 1.0f };

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = m_Vertices;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "Shader//unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "Shader//unlitTexturePS.cso");


}


void Polygon3D::Uninit()
{
	m_VertexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}


void Polygon3D::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	//マテリアル設定
	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);


	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);


	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);


	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//ポリゴン描写
	Renderer::SetDepthEnable(false);
	Renderer::GetDeviceContext()->Draw(4, 0);
	Renderer::SetDepthEnable(true);

}


void Polygon3D::SetUV(float u0, float v0, float u1, float v1)
{
	m_Vertices[0].TexCoord = { u0, v0 };
	m_Vertices[1].TexCoord = { u1, v0 };
	m_Vertices[2].TexCoord = { u0, v1 };
	m_Vertices[3].TexCoord = { u1, v1 };

	Renderer::GetDeviceContext()->UpdateSubresource
	(
		m_VertexBuffer,
		0,
		nullptr,
		m_Vertices,
		0,
		0
	);

}