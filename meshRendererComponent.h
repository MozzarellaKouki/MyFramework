#pragma once

#include "component.h"
#include "mesh.h"
#include "material.h"

class MeshRendererComponent : public Component
{
private:
    static ID3D11InputLayout* s_VertexLayout;
    static ID3D11VertexShader* s_VertexShader;
    static ID3D11PixelShader* s_PixelShader;
    static unsigned int s_ReferenceCount;

    Mesh* m_Mesh = nullptr;
    MaterialData m_Material;
    bool m_Initialized = false;

public:
    void Init() override;
    void Uninit() override;
    void Draw() override;

    void SetMaterial(const MaterialData& material)
    {
        m_Material = material;
    }

    void SetMesh(Mesh* mesh)
    {
        m_Mesh = mesh;
    }

    MaterialData& GetMaterial()
    {
        return m_Material;
    }
};