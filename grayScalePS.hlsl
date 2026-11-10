#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


//白黒の強さを受け取る定数バッファ
cbuffer GrayScaleBuffer : register(b8)
{
    float g_Intensity;
    float3 padding;
};


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
     //元のシーン画像を取得
    float4 color = g_Texture.Sample( g_SamplerState, In.TexCoord );

    //明るさを計算
    float brightness = dot( color.rgb, float3(0.299f, 0.587f, 0.114f) );

    //元の色と白黒の色をIntensityで混ぜる
    float3 grayscale = float3(brightness, brightness, brightness);
    float3 result = lerp(color.rgb, grayscale, g_Intensity);

    outDiffuse = float4(result, color.a);

}

