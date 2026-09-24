#pragma once
#include "main.h"
#include "renderer.h"
#include <vector>

class Mesh
{
private:

    ID3D11Buffer* m_VertexBuffer = nullptr;
    ID3D11Buffer* m_IndexBuffer = nullptr;
    UINT m_IndexCount = 0;

public:

    void Create ( std::vector<VERTEX_3D>& vertices, std::vector<UINT>& indices );
    void Draw();
    void Uninit();
};