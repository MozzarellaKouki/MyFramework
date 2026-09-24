#include "main.h"
#include "renderer.h"
#include "textureManager.h"
#include "polygon2D.h"



void Polygon2D::Init()
{

	m_Vertex[0].Position = XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	m_Vertex[0].Normal = XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	m_Vertex[0].Diffuse = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
	m_Vertex[0].TexCoord = XMFLOAT2{ 0.0f, 0.0f };

	m_Vertex[1].Position = XMFLOAT3{ 1.0f, 0.0f, 0.0f };
	m_Vertex[1].Normal = XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	m_Vertex[1].Diffuse = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
	m_Vertex[1].TexCoord = XMFLOAT2{ 1.0f, 0.0f };

	m_Vertex[2].Position = XMFLOAT3{ 0.0f, 1.0f, 0.0f };
	m_Vertex[2].Normal = XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	m_Vertex[2].Diffuse = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
	m_Vertex[2].TexCoord = XMFLOAT2{ 0.0f, 1.0f };

	m_Vertex[3].Position = XMFLOAT3{ 1.0f, 1.0f, 0.0f };
	m_Vertex[3].Normal = XMFLOAT3{ 0.0f, 0.0f, 0.0f };
	m_Vertex[3].Diffuse = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
	m_Vertex[3].TexCoord = XMFLOAT2{ 1.0f, 1.0f };



	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = m_Vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "Shader//unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "Shader//unlitTexturePS.cso");
}


void Polygon2D::Uninit()
{
	m_VertexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}


void Polygon2D::Update()
{

}


void Polygon2D::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	XMMATRIX world =
		XMMatrixScaling(m_Width, m_Height, 1.0f) *
		XMMatrixTranslation(m_PosX, m_PosY, 0.0f);

	Renderer::SetWorldMatrix(world);
	Renderer::SetBlendMode(BlendMode::Alpha);


	//マテリアル設定
	MATERIAL material{};
	material.Diffuse = { m_Color.x, m_Color.y, m_Color.z, m_Alpha };
	material.TextureEnable = m_TextureEnable;
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
	Renderer::GetDeviceContext()->Draw(4, 0);




}




void  Polygon2D::SetTexture(ID3D11ShaderResourceView* texture)
{
	m_Texture = texture;
	m_TextureEnable = (texture != nullptr);
}

void  Polygon2D::SetColor(float r, float g, float b)
{
	m_Color.x = r;
	m_Color.y = g;
	m_Color.z = b;
}

void Polygon2D::SetUV(float u0, float v0, float u1, float v1)
{
	m_Vertex[0].TexCoord = { u0, v0 };
	m_Vertex[1].TexCoord = { u1, v0 };
	m_Vertex[2].TexCoord = { u0, v1 };
	m_Vertex[3].TexCoord = { u1, v1 };

	Renderer::GetDeviceContext()->UpdateSubresource
	(
		m_VertexBuffer,
		0,
		nullptr,
		m_Vertex,
		0,
		0
	);
}