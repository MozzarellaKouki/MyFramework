#pragma once
#include "main.h"
#include "renderer.h"

class Polygon2D
{

private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	bool m_TextureEnable = true;
	ID3D11ShaderResourceView* m_Texture; 
	VERTEX_3D m_Vertex[4];


	float m_PosX = 0.0f;
	float m_PosY = 0.0f;

	float m_Width = 200.0f;
	float m_Height = 200.0f;

	float m_Alpha = 1.0f;
	XMFLOAT4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };



public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetTexture(ID3D11ShaderResourceView* texture);
	void SetPosition(float x, float y) { m_PosX = x; m_PosY = y; }
	void SetSize(float width, float height) { m_Width = width; m_Height = height; }
	void SetAlpha(float alpha) { m_Alpha = alpha; };
	void SetColor(float r, float g, float b);
	void SetUV(float u0, float v0, float u1, float v1);


};
