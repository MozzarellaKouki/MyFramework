#pragma once

#include "gameObject.h"
#include "heightFieldData.h"
#include <memory>

class Mesh;
class MeshRendererComponent;
class HeightFieldSurfaceComponent;

class Field : public GameObject
{
private:
    HeightFieldData m_HeightField;
    std::unique_ptr<Mesh> m_Mesh;
    MeshRendererComponent* m_MeshRenderer = nullptr;
    HeightFieldSurfaceComponent* m_Surface = nullptr;
    bool m_Initialized = false;

public:
    explicit Field(const HeightFieldConfig& config = {});

    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void Uninit() override;

    HeightFieldData& GetHeightFieldData() { return m_HeightField; }
    const HeightFieldData& GetHeightFieldData() const { return m_HeightField; }
    bool RebuildMesh();
};
