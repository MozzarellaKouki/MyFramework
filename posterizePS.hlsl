#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 color = g_Texture.Sample(g_SamplerState, In.TexCoord);
    color *= In.Diffuse;

    float levels = max(PosterizeLevels, 2.0f);
    color.rgb = floor(color.rgb * levels) / levels;

    outDiffuse = color;
}
