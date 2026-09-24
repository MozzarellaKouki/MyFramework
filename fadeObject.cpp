#include "fadeObject.h"
#include "main.h"

void FadeObject::Init()
{
    m_Polygon.Init();

    m_Polygon.SetPosition(0.0f, 0.0f);
    m_Polygon.SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    m_Polygon.SetAlpha(0.0f);

    m_Polygon.SetTexture(nullptr);
    m_Polygon.SetColor(0.0f, 0.0f, 0.0f);
}


void FadeObject::Uninit()
{
    m_Polygon.Uninit();
}


void FadeObject::Update(float deltaTime)
{
    if (!m_Fading) return;

    if (m_FadeIn)
    {
        m_Alpha -= m_FadeSpeed * deltaTime;

        if (m_Alpha <= 0.0f)
        {
            m_Alpha = 0.0f;
            m_Fading = false;
        }
    }
    else
    {
        m_Alpha += m_FadeSpeed * deltaTime;

        if (m_Alpha >= 1.0f)
        {
            m_Alpha = 1.0f;
            m_Fading = false;
        }
    }

    m_Polygon.SetAlpha(m_Alpha);
}


void FadeObject::Draw()
{
    m_Polygon.Draw();
}





void FadeObject::FadeIn()
{

    m_Alpha = 1.0f;
    m_Polygon.SetAlpha(m_Alpha);

    m_FadeIn = true;
    m_Fading = true;
}

void FadeObject::FadeOut()
{
    m_Polygon.SetColor(0.0f, 0.0f, 0.0f);

    m_Alpha = 0.0f;
    m_Polygon.SetAlpha(m_Alpha);

    m_FadeIn = false;
    m_Fading = true;
}

void FadeObject::FadeOut(float speed)
{
    m_Alpha = 0.0f;
    m_Polygon.SetAlpha(m_Alpha);

    m_FadeSpeed = speed;

    m_FadeIn = false;
    m_Fading = true;
}


void FadeObject::FadeOutWhite()
{
    m_Polygon.SetColor(1.0f, 1.0f, 1.0f);

    m_Alpha = 0.0f;
    m_Polygon.SetAlpha(m_Alpha);

    m_FadeIn = false;
    m_Fading = true;
}

void FadeObject::FadeOutRed()
{
    m_Polygon.SetColor(1.0f, 0.0f, 0.0f);

    m_Alpha = 0.0f;
    m_Polygon.SetAlpha(m_Alpha);

    m_FadeIn = false;
    m_Fading = true;
}

