#include "modelLoader.h"
#include <fstream>
#include <sstream>

Mesh* ModelLoader::LoadOBJ(const char* fileName)
{
    //OBJの一時保存用
    std::vector<XMFLOAT3>positions;
    std::vector<XMFLOAT2>texcoords;

    //メッシュの最終決定
    std::vector<VERTEX_3D>vertices;
    std::vector<UINT>indices;

    //--------------------------------
    // ファイル開く
    //--------------------------------

    std::ifstream file(fileName);

    if (!file.is_open())
    {
        return nullptr;
    }

    //--------------------------------
    //           1行ずつ読む
    //--------------------------------

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;

        ss >> type;

        //--------------------------------
        //           頂点座標
        //--------------------------------

        if (type == "v")
        {
            XMFLOAT3 pos{};

            ss  >> pos.x
                >> pos.y
                >> pos.z;

            positions.push_back(pos);
        }

        //--------------------------------
        //             UV
        //--------------------------------

        else if (type == "vt")
        {
            XMFLOAT2 uv{};

            ss >> uv.x
                >> uv.y;

            uv.y = 1.0f - uv.y;

            texcoords.push_back(uv);
        }

        //--------------------------------
        // 面
        //--------------------------------

        else if (type == "f")
        {
            for (int i = 0; i < 3; i++)
            {
                std::string vertStr;

                ss >> vertStr;

                //--------------------------------
                // "1/2"
                //--------------------------------

                std::stringstream vs(
                    vertStr
                );

                std::string p, t;

                std::getline(vs, p, '/');

                std::getline(vs, t, '/');

                int posIndex =
                    std::stoi(p);

                int texIndex =
                    std::stoi(t);

                //--------------------------------
                // Vertex生成
                //--------------------------------

                VERTEX_3D v{};

                v.Position =
                    positions[posIndex - 1];

                v.TexCoord =
                    texcoords[texIndex - 1];

                v.Normal =
                { 0,0,0 };

                v.Diffuse =
                { 1,1,1,1 };

                vertices.push_back(v);

                indices.push_back(
                    (UINT)indices.size()
                );
            }
        }
    }


    Mesh* mesh = new Mesh();
    mesh->Create(vertices, indices);

    return mesh;
}