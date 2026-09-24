#include "mesh.h"

void Mesh::Create(std::vector<VERTEX_3D>& vertices, std::vector<UINT>& indices)
{

    // ’¸“_”•Û‘¶
    m_IndexCount = (UINT)indices.size();

    //--------------------------------
    //       VertexBufferì¬
    //--------------------------------

    D3D11_BUFFER_DESC bd{};

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * (UINT)vertices.size();
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sd{};

    sd.pSysMem = vertices.data();

    Renderer::GetDevice()-> CreateBuffer(&bd, &sd, &m_VertexBuffer);

    //--------------------------------
    // IndexBufferì¬
    //--------------------------------

    D3D11_BUFFER_DESC ibd{};

    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = sizeof(UINT) * (UINT)indices.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA isd{};

    isd.pSysMem = indices.data();

    Renderer::GetDevice()-> CreateBuffer(&ibd, &isd, &m_IndexBuffer);
}


void Mesh::Draw()
{
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;

    //--------------------------------
    // VertexBufferÝ’è
    //--------------------------------

    Renderer::GetDeviceContext()->
        IASetVertexBuffers(
            0,
            1,
            &m_VertexBuffer,
            &stride,
            &offset
        );

    //--------------------------------
    // IndexBufferÝ’è
    //--------------------------------

    Renderer::GetDeviceContext()-> IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    //--------------------------------
    // PrimitiveTopology
    //--------------------------------

    Renderer::GetDeviceContext()-> IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //--------------------------------
    // Draw
    //--------------------------------

    Renderer::GetDeviceContext()-> DrawIndexed(m_IndexCount, 0, 0 );
}


void Mesh::Uninit()
{
    if (m_VertexBuffer)
    {
        m_VertexBuffer->Release();
        m_VertexBuffer = nullptr;
    }

    if (m_IndexBuffer)
    {
        m_IndexBuffer->Release();
        m_IndexBuffer = nullptr;
    }
}