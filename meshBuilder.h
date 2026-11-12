#pragma once

#include "mesh.h"
#include <vector>

class MeshBuilder
{
public:
    static Mesh* CreateCube();
    static bool CreateHeightField(
        int segmentsX,
        int segmentsZ,
        float width,
        float depth,
        const std::vector<float>& heights,
        MeshData& output);
};