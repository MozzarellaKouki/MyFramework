#include "heightFieldData.h"

#include <cmath>
#include <limits>

HeightFieldData::HeightFieldData(const HeightFieldConfig& config)
{
    Initialize(config);
}

bool HeightFieldData::Initialize(const HeightFieldConfig& config)
{
    if (config.segmentsX <= 0 || config.segmentsZ <= 0 ||
        !std::isfinite(config.width) || !std::isfinite(config.depth) ||
        config.width <= 0.0f || config.depth <= 0.0f)
    {
        return false;
    }

    const size_t countX = static_cast<size_t>(config.segmentsX) + 1;
    const size_t countZ = static_cast<size_t>(config.segmentsZ) + 1;
    if (countX > std::numeric_limits<size_t>::max() / countZ)
        return false;

    const size_t sampleCount = countX * countZ;
    if (sampleCount > std::numeric_limits<unsigned int>::max())
        return false;

    m_Config = config;
    m_Heights.assign(sampleCount, 0.0f);
    return true;
}

std::size_t HeightFieldData::GetIndex(int x, int z) const
{
    const std::size_t countX = static_cast<std::size_t>(m_Config.segmentsX) + 1;
    return static_cast<std::size_t>(z) * countX + static_cast<std::size_t>(x);
}

bool HeightFieldData::SetHeight(int x, int z, float height)
{
    if (x < 0 || x > m_Config.segmentsX ||
        z < 0 || z > m_Config.segmentsZ || !std::isfinite(height) ||
        m_Heights.empty())
    {
        return false;
    }

    m_Heights[GetIndex(x, z)] = height;
    return true;
}

bool HeightFieldData::TryGetHeight(int x, int z, float& height) const
{
    if (x < 0 || x > m_Config.segmentsX ||
        z < 0 || z > m_Config.segmentsZ || m_Heights.empty())
    {
        return false;
    }

    height = m_Heights[GetIndex(x, z)];
    return true;
}
