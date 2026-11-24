#pragma once

#include "component.h"
#include "mesh.h"
#include "material.h"
#include "resourceHandle.h"

class Transform;

class MeshRendererComponent : public Component
{
private:
    static ID3D11InputLayout* s_VertexLayout;
    static ID3D11VertexShader* s_VertexShader;
    static ID3D11PixelShader* s_PixelShader;
    static unsigned int s_ReferenceCount;

    Handle<Mesh> m_MeshHandle;  //共有メッシュ
    Mesh* m_Mesh = nullptr;       //借りている専用メッシュ（ハンドルとどちらか一方）
    MaterialData m_Material;
    bool m_Initialized = false;
    Transform* m_Transform = nullptr;

public:
    void Init() override;
    void Uninit() override;
    void Draw() override;

    void SetMaterial(const MaterialData& material)
    {
        m_Material = material;
    }

    //共有メッシュ（ResourceManager が持ち主）を使う
    void SetMesh(Handle<Mesh> mesh)
    {
        m_MeshHandle = mesh;
        m_Mesh = nullptr;
    }

    //持ち主が自分で管理しているメッシュ（Field の地形など）を借りる
    //持ち主はこのコンポーネントより長生きすること
    void SetMesh(Mesh* mesh)
    {
        m_Mesh = mesh;
        m_MeshHandle = {};
    }

    MaterialData& GetMaterial()
    {
        return m_Material;
    }
};