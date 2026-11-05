#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // 元のシーン画像を取得
    float4 color = g_Texture.Sample(
        g_SamplerState,
        In.TexCoord
    );

    // 明るさを計算
    float brightness = dot(
        color.rgb,
        float3(0.299f, 0.587f, 0.114f)
    );

    // 明るい部分だけを残す
    const float threshold = 0.7f;

    if (brightness >= threshold)
    {
        outDiffuse = float4(color.rgb, 1.0f);
    }
    else
    {
        outDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}