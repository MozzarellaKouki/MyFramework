#pragma once

#include "polygon2D.h"

class FadeObject
{
private:
    Polygon2D m_Polygon;

    float m_Alpha = 0.0f;
    float m_FadeSpeed = 1.0f;

    bool m_Fading = false;
    bool m_FadeIn = false;

public:
    void Init();
    void Update(float deltaTime);
    void Draw();
    void Uninit();

    void FadeIn();
    void FadeOut();
    void FadeOut(float speed);
    void FadeOutWhite();
    void FadeOutRed();

    bool IsFading() const { return m_Fading; }
    bool IsFadeIn() const { return m_FadeIn; }
};
