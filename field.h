#pragma once
#include "gameObject.h"
#include "renderer.h"

class Field : public GameObject
{
private:
    ID3D11Buffer* m_VertexBuffer;

    ID3D11InputLayout* m_VertexLayout;
    ID3D11VertexShader* m_VertexShader;
    ID3D11PixelShader* m_PixelShader;

    ID3D11ShaderResourceView* m_Texture;

    // ”g‚Ì’¸“_”
    static const int GRID_SIZE = 30;

    // ”g‚ÌŠÔ
    float m_Time = 0.0f;

    // ì¬‚µ‚½’¸“_‚ğ•Û
    std::vector<VERTEX_3D> m_Vertices;

public:
    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Uninit() override;
};