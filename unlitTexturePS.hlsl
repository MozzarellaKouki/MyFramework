#include "common.hlsl"

Texture2D     g_Texture      : register(t0);
SamplerState  g_SamplerState : register(s0);



void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
        outDiffuse *= In.Diffuse;

        outDiffuse.rgb =
            floor(outDiffuse.rgb * PosterizeLevels)
            / PosterizeLevels;
    }
    else
    {
        outDiffuse = In.Diffuse;

        float levels = 16.0f;

        outDiffuse.rgb =
            floor(outDiffuse.rgb * levels) / levels;
    }
}