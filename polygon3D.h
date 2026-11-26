#pragma once
#include "main.h"
#include "renderer.h"
#include "resourceHandle.h"
#include "texture.h"

class Polygon3D
{

private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	Handle<Texture> m_Texture;

	VERTEX_3D m_Vertices[4];


public:
	void Init();
	void Uninit();
	void Draw();


	void SetTexture(Handle<Texture> texture) { m_Texture = texture; }
	void SetUV(float u0, float v0, float u1, float v1);


};
