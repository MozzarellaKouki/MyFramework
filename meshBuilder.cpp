#include "meshBuilder.h"

#include <cmath>
#include <limits>
#include <utility>

Mesh* MeshBuilder::CreateCube()
{
    std::vector<VERTEX_3D> vertices;
    std::vector<UINT> indices;

    Mesh* mesh = new Mesh();
    if (!mesh->Create(vertices, indices))
    {
        delete mesh;
        return nullptr;
    }

    return mesh;
}

bool MeshBuilder::CreateHeightField(
    int segmentsX,
    int segmentsZ,
    float width,
    float depth,
    const std::vector<float>& heights,
    MeshData& output)
{
    if (segmentsX <= 0 || segmentsZ <= 0 ||
        !std::isfinite(width) || !std::isfinite(depth) ||
        width <= 0.0f || depth <= 0.0f)
    {
        return false;
    }

    const size_t countX = static_cast<size_t>(segmentsX) + 1;
    const size_t countZ = static_cast<size_t>(segmentsZ) + 1;
    if (countX > std::numeric_limits<size_t>::max() / countZ)
        return false;

    const size_t vertexCount = countX * countZ;
    const size_t cellCount = static_cast<size_t>(segmentsX) *
        static_cast<size_t>(segmentsZ);
    if (vertexCount != heights.size() ||
        vertexCount > std::numeric_limits<UINT>::max() ||
        cellCount > std::numeric_limits<UINT>::max() / 6)
    {
        return false;
    }

    MeshData generated;
    generated.vertices.resize(vertexCount);
    generated.indices.reserve(cellCount * 6);

    for (int z = 0; z <= segmentsZ; ++z)
    {
        for (int x = 0; x <= segmentsX; ++x)
        {
            const size_t index = static_cast<size_t>(z) * countX + x;
            const float u = static_cast<float>(x) / segmentsX;
            const float v = static_cast<float>(z) / segmentsZ;

            VERTEX_3D& vertex = generated.vertices[index];
            vertex.Position = XMFLOAT3(
                -width * 0.5f + width * u,
                heights[index],
                depth * 0.5f - depth * v);
            vertex.Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
            vertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            vertex.TexCoord = XMFLOAT2(u, v);
        }
    }

    for (int z = 0; z < segmentsZ; ++z)
    {
        for (int x = 0; x < segmentsX; ++x)
        {
            const UINT topLeft = static_cast<UINT>(
                static_cast<size_t>(z) * countX + x);
            const UINT topRight = topLeft + 1;
            const UINT bottomLeft = static_cast<UINT>(
                static_cast<size_t>(z + 1) * countX + x);
            const UINT bottomRight = bottomLeft + 1;

            generated.indices.push_back(topLeft);
            generated.indices.push_back(topRight);
            generated.indices.push_back(bottomLeft);

            generated.indices.push_back(bottomLeft);
            generated.indices.push_back(topRight);
            generated.indices.push_back(bottomRight);
        }
    }

    for (size_t i = 0; i < generated.indices.size(); i += 3)
    {
        VERTEX_3D& a = generated.vertices[generated.indices[i]];
        VERTEX_3D& b = generated.vertices[generated.indices[i + 1]];
        VERTEX_3D& c = generated.vertices[generated.indices[i + 2]];

        const float abX = b.Position.x - a.Position.x;
        const float abY = b.Position.y - a.Position.y;
        const float abZ = b.Position.z - a.Position.z;
        const float acX = c.Position.x - a.Position.x;
        const float acY = c.Position.y - a.Position.y;
        const float acZ = c.Position.z - a.Position.z;

        const float normalX = abY * acZ - abZ * acY;
        const float normalY = abZ * acX - abX * acZ;
        const float normalZ = abX * acY - abY * acX;

        for (size_t corner = 0; corner < 3; ++corner)
        {
            VERTEX_3D& vertex = generated.vertices[generated.indices[i + corner]];
            vertex.Normal.x += normalX;
            vertex.Normal.y += normalY;
            vertex.Normal.z += normalZ;
        }
    }

    for (VERTEX_3D& vertex : generated.vertices)
    {
        const float length = std::sqrt(
            vertex.Normal.x * vertex.Normal.x +
            vertex.Normal.y * vertex.Normal.y +
            vertex.Normal.z * vertex.Normal.z);

        if (length > 0.0f)
        {
            vertex.Normal.x /= length;
            vertex.Normal.y /= length;
            vertex.Normal.z /= length;
        }
        else
        {
            vertex.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        }
    }

    output = std::move(generated);
    return true;
}