#pragma once

#include "mesh.h"
#include <filesystem>
#include <memory>

class ModelLoader
{
public:

    //読み込んだメッシュの持ち主は呼び出し側（通常は ResourceManager）
    static std::unique_ptr<Mesh> LoadOBJ(const std::filesystem::path& fileName);
};
