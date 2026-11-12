#include "heightFieldSurfaceComponent.h"

#include "gameObject.h"
#include "heightFieldData.h"
#include "transformComponent.h"

#include <cmath>

void HeightFieldSurfaceComponent::Init()
{
    m_Transform = m_Owner->GetComponent<TransformComponent>();
}

bool HeightFieldSurfaceComponent::TryGetHeight(
    float worldX,
    float worldZ,
    float& worldHeight) const
{
    if (!m_HeightField || !m_Transform ||
        !std::isfinite(worldX) || !std::isfinite(worldZ))
    {
        return false;
    }

    const HeightFieldConfig& config = m_HeightField->GetConfig();
    const Vector3 position = m_Transform->GetPosition();
    const Vector3 scale = m_Transform->GetScale();
    if (scale.x == 0.0f || scale.y == 0.0f || scale.z == 0.0f)
        return false;

    const float localX = (worldX - position.x) / scale.x;
    const float localZ = (worldZ - position.z) / scale.z;
    const float u = (localX + config.width * 0.5f) / config.width;
    const float v = (config.depth * 0.5f - localZ) / config.depth;
    if (u < 0.0f || u > 1.0f || v < 0.0f || v > 1.0f)
        return false;

    const float gridX = u * config.segmentsX;
    const float gridZ = v * config.segmentsZ;
    const int x = (gridX >= config.segmentsX)
        ? config.segmentsX - 1 : static_cast<int>(gridX);
    const int z = (gridZ >= config.segmentsZ)
        ? config.segmentsZ - 1 : static_cast<int>(gridZ);
    const float tx = gridX - x;
    const float tz = gridZ - z;

    float topLeft = 0.0f;
    float topRight = 0.0f;
    float bottomLeft = 0.0f;
    float bottomRight = 0.0f;
    if (!m_HeightField->TryGetHeight(x, z, topLeft) ||
        !m_HeightField->TryGetHeight(x + 1, z, topRight) ||
        !m_HeightField->TryGetHeight(x, z + 1, bottomLeft) ||
        !m_HeightField->TryGetHeight(x + 1, z + 1, bottomRight))
    {
        return false;
    }

    // Keep the query surface aligned with MeshBuilder's TL-TR-BL / BL-TR-BR split.
    float localHeight = 0.0f;
    if (tx + tz <= 1.0f)
    {
        localHeight = topLeft * (1.0f - tx - tz) +
            topRight * tx + bottomLeft * tz;
    }
    else
    {
        localHeight = bottomLeft * (1.0f - tx) +
            topRight * (1.0f - tz) +
            bottomRight * (tx + tz - 1.0f);
    }

    worldHeight = position.y + localHeight * scale.y;
    return std::isfinite(worldHeight);
}
