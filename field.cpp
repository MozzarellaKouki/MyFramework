#include "main.h"
#include "field.h"
#include "mesh.h"
#include "meshBuilder.h"
#include "meshRendererComponent.h"
#include "heightFieldSurfaceComponent.h"
#include "textureManager.h"
#include "transformComponent.h"

#include <utility>
#include <cmath>

Field::Field(const HeightFieldConfig& config)
    : m_HeightField(config)
{
}

void Field::Init()
{
    if (m_Initialized)
        return;

    // A few smooth rises and a shallow basin give the initial field visible height variation.
    const HeightFieldConfig& config = m_HeightField.GetConfig();
    for (int z = 0; z <= config.segmentsZ; ++z)
    {
        for (int x = 0; x <= config.segmentsX; ++x)
        {
            const float localX = -config.width * 0.5f +
                config.width * static_cast<float>(x) / config.segmentsX;
            const float localZ = config.depth * 0.5f -
                config.depth * static_cast<float>(z) / config.segmentsZ;
            const auto hill = [](float x, float z, float centerX, float centerZ,
                float radius, float height)
            {
                const float dx = x - centerX;
                const float dz = z - centerZ;
                return height * std::exp(-(dx * dx + dz * dz) / radius);
            };

            const float height =
                hill(localX, localZ, -0.45f, 0.25f, 0.12f, 0.35f) +
                hill(localX, localZ, 0.45f, -0.25f, 0.18f, 0.50f) -
                hill(localX, localZ, 0.05f, 0.05f, 0.30f, 0.12f);
            m_HeightField.SetHeight(x, z, height);
        }
    }

    if (!RebuildMesh())
    {
        MessageBox(nullptr, "Field mesh generation failed.", "Error", MB_OK);
        return;
    }

    auto* transform = AddComponent<TransformComponent>();
    transform->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
    transform->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
    transform->SetScale(Vector3(10.0f, 10.0f, 10.0f));

    m_Surface = AddComponent<HeightFieldSurfaceComponent>();
    m_Surface->SetHeightField(&m_HeightField);

    m_MeshRenderer = AddComponent<MeshRendererComponent>();
    m_MeshRenderer->SetMesh(m_Mesh.get());

    MaterialData material{};
    material.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    material.texture = TextureManager::Load(L"Asset/texture/MoonSurface.png");
    m_MeshRenderer->SetMaterial(material);

    m_Initialized = true;
}

void Field::Update(float dt)
{
    GameObject::Update(dt);
}

void Field::Draw()
{
    if (m_Initialized) GameObject::Draw();
}

void Field::Uninit()
{
    GameObject::Uninit();
    m_MeshRenderer = nullptr;
    m_Surface = nullptr;
    m_Mesh.reset();
    m_Initialized = false;
}

bool Field::RebuildMesh()
{
    const HeightFieldConfig& config = m_HeightField.GetConfig();
    MeshData meshData;
    if (!MeshBuilder::CreateHeightField(
        config.segmentsX,
        config.segmentsZ,
        config.width,
        config.depth,
        m_HeightField.GetHeights(),
        meshData))
    {
        return false;
    }

    if (!m_Mesh)
        m_Mesh = std::make_unique<Mesh>();

    if (!m_Mesh->Create(meshData.vertices, meshData.indices))
        return false;

    if (m_MeshRenderer)
        m_MeshRenderer->SetMesh(m_Mesh.get());

    return true;
}
