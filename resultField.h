#pragma once
#include "gameObject.h"


class Field2 : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;


public:
	void Init() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void Uninit() override;

};

