#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // 縦方向の1ピクセル分のUVサイズを取得
    float2 offset = float2(
        0.0f,
        abs(ddy(In.TexCoord.y))
    );

    float2 uv = In.TexCoord;

    outDiffuse = g_Texture.Sample(
        g_SamplerState, uv
    ) * 0.40f;

    outDiffuse += g_Texture.Sample(
        g_SamplerState, uv + offset
    ) * 0.24f;

    outDiffuse += g_Texture.Sample(
        g_SamplerState, uv - offset
    ) * 0.24f;

    outDiffuse += g_Texture.Sample(
        g_SamplerState, uv + offset * 2.0f
    ) * 0.06f;

    outDiffuse += g_Texture.Sample(
        g_SamplerState, uv - offset * 2.0f
    ) * 0.06f;
}