#include "meshBuilder.h"


Mesh* MeshBuilder::CreateCube()
{
    std::vector<VERTEX_3D> vertices;

    std::vector<UINT> indices;

    // ’¸“_’Ç‰Á
    // index’Ç‰Á

    Mesh* mesh = new Mesh();

    mesh->Create(vertices, indices);

    return mesh;
}