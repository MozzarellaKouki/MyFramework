#include "main.h"
#include "field.h"
#include "renderer.h"
#include "textureManager.h"
#include "keyboard.h"
#include "particleTemperSettings.h"

#include "transformComponent.h"
#include "particleEmitterComponent.h"


void Field::Init()
{
	m_Vertices.clear();
	const float fieldSize = 2.0f;

	for (int z = 0; z < m_GridSize; z++)
	{
		for (int x = 0; x < m_GridSize; x++)
		{
			float x0 =
				-fieldSize / 2.0f +
				fieldSize * x / m_GridSize;

			float x1 =
				-fieldSize / 2.0f +
				fieldSize * (x + 1) / m_GridSize;

			float z0 =
				fieldSize / 2.0f -
				fieldSize * z / m_GridSize;

			float z1 =
				fieldSize / 2.0f -
				fieldSize * (z + 1) / m_GridSize;


			// 左上
			VERTEX_3D v0{};
			v0.Position = XMFLOAT3(x0, 0.0f, z0);
			v0.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			v0.Diffuse = XMFLOAT4(1, 1, 1, 1);
			v0.TexCoord = XMFLOAT2(
				(float)x / m_GridSize,
				(float)z / m_GridSize
			);

			// 右上
			VERTEX_3D v1{};
			v1.Position = XMFLOAT3(x1, 0.0f, z0);
			v1.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			v1.Diffuse = XMFLOAT4(1, 1, 1, 1);
			v1.TexCoord = XMFLOAT2(
				(float)(x + 1) / m_GridSize,
				(float)z / m_GridSize
			);

			// 左下
			VERTEX_3D v2{};
			v2.Position = XMFLOAT3(x0, 0.0f, z1);
			v2.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			v2.Diffuse = XMFLOAT4(1, 1, 1, 1);
			v2.TexCoord = XMFLOAT2(
				(float)x / m_GridSize,
				(float)(z + 1) / m_GridSize
			);

			// 右下
			VERTEX_3D v3{};
			v3.Position = XMFLOAT3(x1, 0.0f, z1);
			v3.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			v3.Diffuse = XMFLOAT4(1, 1, 1, 1);
			v3.TexCoord = XMFLOAT2(
				(float)(x + 1) / m_GridSize,
				(float)(z + 1) / m_GridSize
			);


			m_Vertices.push_back(v0);
			m_Vertices.push_back(v1);
			m_Vertices.push_back(v2);

			m_Vertices.push_back(v2);
			m_Vertices.push_back(v1);
			m_Vertices.push_back(v3);
		}
	}

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.ByteWidth =
		sizeof(VERTEX_3D) * m_Vertices.size();
	bd.BindFlags =
		D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = m_Vertices.data();

	Renderer::GetDevice()->CreateBuffer
	(
		&bd,
		&sd,
		&m_VertexBuffer
	);

	// シェーダー読込
	Renderer::CreateVertexShader
	(
		&m_VertexShader,
		&m_VertexLayout,
		"shader\\unlitTextureVS.cso"
	);

	Renderer::CreatePixelShader
	(
		&m_PixelShader,
		"shader\\unlitTexturePS.cso"
	);

	// テクスチャ
	m_Texture =
		TextureManager::Load
		(
			L"Asset/texture/MoonSurface.png"
		);

	// トランスフォーム設定
	auto transform =
		AddComponent<TransformComponent>();

	transform->SetPosition(Vector3(0, 0, 5));
	transform->SetRotation(Vector3(0, 0, 0));
	transform->SetScale(Vector3(10, 10, 10));
}

void Field::Uninit()
{

	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void Field::Update(float dt)
{
	if (m_WaveEnabled)
	{
		UpdateWave(dt);
	}
}


void Field::Draw()
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

	material.Diffuse =	{ 1,1,1,1 };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// Texture
	Renderer::GetDeviceContext()-> PSSetShaderResources(0, 1, &m_Texture);

	// VertexBuffer
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->
		IASetVertexBuffers
		(
			0,
			1,
			&m_VertexBuffer,
			&stride,
			&offset
		);

	// Topology
	Renderer::GetDeviceContext()->IASetPrimitiveTopology
	(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);

	// Draw
	Renderer::GetDeviceContext()->Draw
	(
		m_GridSize * m_GridSize * 6,
		0
	);
}


// 波 ==============================================
void Field::UpdateWave(float dt)
{
	{
		m_WaveTime += dt;

		// 頂点のY座標を波の形にする
		for (auto& vertex : m_Vertices)
		{
			float x = vertex.Position.x;

			vertex.Position.y =
				sinf(x * 0.5f + m_WaveTime) * 2.0f;
		}

		// GPUへ頂点データを送る
		D3D11_MAPPED_SUBRESOURCE mapped{};

		Renderer::GetDeviceContext()->Map
		(
			m_VertexBuffer,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mapped
		);

		memcpy
		(
			mapped.pData,
			m_Vertices.data(),
			sizeof(VERTEX_3D) * m_Vertices.size()
		);


		Renderer::GetDeviceContext()->Unmap
		(
			m_VertexBuffer,
			0
		);
	}
}