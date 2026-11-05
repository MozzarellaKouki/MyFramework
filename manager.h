#pragma once

#include "RenderTexture.h"

class Bloom;

class Manager
{
private:
    static RenderTexture m_RenderTexture;
    static Bloom* m_Bloom;

public:
    static void Init();
    static void Uninit();
    static void Update();
    static void Draw();
};