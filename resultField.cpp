#include "main.h"
#include "resultfield.h"
#include "renderer.h"
#include "textureManager.h"
#include "keyboard.h"
#include "transformComponent.h"


void Field2::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-50.0f, 0.0f, 30.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(50.0f, 0.0f, 30.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-50.0f, 0.0f, -30.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(50.0f, 0.0f, -30.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// シェーダー読込
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	// テクスチャ
	m_Texture = TextureManager::Load(L"Asset/texture/Clear.png");
	//m_Texture = TextureManager::Load(L"Asset/texture/MoonSurface.png");

	//トランスフォーム設定
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(Vector3(0, 0, 5));
	transform->SetRotation(Vector3(0, 0, 0));
	transform->SetScale(Vector3(10, 10, 10));


}

void Field2::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void Field2::Update(float deltaTime)
{

}


void Field2::Draw()
{

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	auto transform = GetComponent<TransformComponent>();
	if (transform)
	{
		Renderer::SetWorldMatrix(transform->GetWorldMatrix());
	}

	//Animation設定
	SPRITE_ANIMATION anim{};
	anim.Frame = 0;
	anim.Columns = 1;
	anim.Rows = 1;
	Renderer::SetSpriteAnimation(anim);

	// Material設定
	MATERIAL material{};

	material.Diffuse = { 1,1,1,1 };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// Texture
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// VertexBuffer
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->
		IASetVertexBuffers(
			0,
			1,
			&m_VertexBuffer,
			&stride,
			&offset
		);

	// Topology
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Draw
	Renderer::GetDeviceContext()->
		Draw(4, 0);
}