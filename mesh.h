#pragma once

#include "main.h"
#include "renderer.h"
#include <vector>

struct MeshData
{
    std::vector<VERTEX_3D> vertices;
    std::vector<UINT> indices;
};

class Mesh
{
private:
    ID3D11Buffer* m_VertexBuffer = nullptr;
    ID3D11Buffer* m_IndexBuffer = nullptr;
    UINT m_IndexCount = 0;

public:
    Mesh() = default;
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    bool Create(const std::vector<VERTEX_3D>& vertices,
        const std::vector<UINT>& indices);
    void Draw() const;
    void Uninit();
};
