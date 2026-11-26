#include "main.h"
#include "GrayScale.h"
#include "renderer.h"

void GrayScale::Init()
{
    m_Display.Init("Shader//grayScalePS.cso");

    m_Initialized = true;
}

void GrayScale::Uninit()
{
    if (!m_Initialized) return;

    m_Display.Uninit();
    m_Initialized = false;
}

void GrayScale::Update()
{
}

void GrayScale::Draw( ID3D11ShaderResourceView* source, RenderTexture& destination)
{
    if (!m_Initialized || !source) return;


    Renderer::SetGrayscaleIntensity(m_Intensity);

    destination.SetRenderTarget();
    m_Display.Draw(source);
}
