#pragma once

#include "mesh.h"
#include <memory>
#include <vector>

class MeshBuilder
{
public:
    static std::unique_ptr<Mesh> CreateCube();
    static bool CreateHeightField(
        int segmentsX,
        int segmentsZ,
        float width,
        float depth,
        const std::vector<float>& heights,
        MeshData& output);
};