#pragma once

#include "RenderTexture.h"

class Bloom;
class GrayScale;

class Manager
{
private:
    static inline RenderTexture m_RenderTexture;

public:
    static void Init();
    static void Uninit();
    static void Update();
    static void Draw();
};