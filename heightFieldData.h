#pragma once

#include <cstddef>
#include <vector>

struct HeightFieldConfig
{
    int segmentsX = 32;
    int segmentsZ = 32;
    float width = 2.0f;
    float depth = 2.0f;
};

class HeightFieldData
{
private:
    HeightFieldConfig m_Config;
    std::vector<float> m_Heights;

    std::size_t GetIndex(int x, int z) const;

public:
    explicit HeightFieldData(const HeightFieldConfig& config = {});

    bool Initialize(const HeightFieldConfig& config);
    const HeightFieldConfig& GetConfig() const { return m_Config; }
    const std::vector<float>& GetHeights() const { return m_Heights; }

    bool SetHeight(int x, int z, float height);
    bool TryGetHeight(int x, int z, float& height) const;
};
