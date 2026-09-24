#pragma once

#include "mesh.h"

class ModelLoader
{
public:

    static Mesh* LoadOBJ(
        const char* fileName
    );
};
