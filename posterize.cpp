#include "main.h"
#include "posterize.h"
#include "renderer.h"

void Posterize::Init()
{
    if (m_Initialized) return;

    m_Display.Init("Shader//posterizePS.cso");

    m_Initialized = true;
}

void Posterize::Uninit()
{
    if (!m_Initialized) return;

    m_Display.Uninit();
    m_Initialized = false;
}

void Posterize::Draw(ID3D11ShaderResourceView* source, RenderTexture& destination)
{
    if (!m_Initialized || !source) return;

    Renderer::SetPosterizeLevels(m_Levels);

    destination.SetRenderTarget();
    m_Display.Draw(source);
}

void Posterize::SetLevels(float levels)
{
    m_Levels = (levels < 2.0f) ? 2.0f : levels;
}

float Posterize::GetLevels() const
{
    return m_Levels;
}
