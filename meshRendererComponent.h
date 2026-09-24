#pragma once
#include "component.h"
#include "mesh.h"
#include "material.h"
#include "mesh.h"

class MeshRendererComponent : public Component
{
private:

    Mesh* m_Mesh = nullptr;
    MaterialData m_Material;

public:

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

    void Draw() override;
};