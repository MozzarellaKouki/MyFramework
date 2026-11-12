#include "mesh.h"

#include <limits>

Mesh::~Mesh()
{
    Uninit();
}

bool Mesh::Create(
    const std::vector<VERTEX_3D>& vertices,
    const std::vector<UINT>& indices)
{
    Uninit();

    if (vertices.empty() || indices.empty() || indices.size() % 3 != 0 ||
        vertices.size() > std::numeric_limits<UINT>::max() / sizeof(VERTEX_3D) ||
        indices.size() > std::numeric_limits<UINT>::max() / sizeof(UINT))
    {
        return false;
    }

    for (UINT index : indices)
    {
        if (index >= vertices.size())
            return false;
    }

    ID3D11Device* device = Renderer::GetDevice();
    if (!device)
        return false;

    D3D11_BUFFER_DESC vertexDesc{};
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.ByteWidth = static_cast<UINT>(sizeof(VERTEX_3D) * vertices.size());
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData{};
    vertexData.pSysMem = vertices.data();

    if (FAILED(device->CreateBuffer(&vertexDesc, &vertexData, &m_VertexBuffer)))
        return false;

    D3D11_BUFFER_DESC indexDesc{};
    indexDesc.Usage = D3D11_USAGE_DEFAULT;
    indexDesc.ByteWidth = static_cast<UINT>(sizeof(UINT) * indices.size());
    indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData{};
    indexData.pSysMem = indices.data();

    if (FAILED(device->CreateBuffer(&indexDesc, &indexData, &m_IndexBuffer)))
    {
        Uninit();
        return false;
    }

    m_IndexCount = static_cast<UINT>(indices.size());
    return true;
}

void Mesh::Draw() const
{
    if (!m_VertexBuffer || !m_IndexBuffer || m_IndexCount == 0)
        return;

    ID3D11DeviceContext* context = Renderer::GetDeviceContext();
    if (!context)
        return;

    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->DrawIndexed(m_IndexCount, 0, 0);
}

void Mesh::Uninit()
{
    if (m_IndexBuffer)
    {
        m_IndexBuffer->Release();
        m_IndexBuffer = nullptr;
    }

    if (m_VertexBuffer)
    {
        m_VertexBuffer->Release();
        m_VertexBuffer = nullptr;
    }

    m_IndexCount = 0;
}