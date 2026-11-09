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

  //RGBすべてに輝度を設定して白黒化
    outDiffuse = float4( brightness, brightness, brightness,color.a );
}

