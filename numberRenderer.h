#pragma once
#include "Polygon2D.h"

class NumberRenderer
{
private:
    Polygon2D m_NumberSprite;

public:
    void Init();
    void Uninit();

    void Draw(int number, float x, float y);
};