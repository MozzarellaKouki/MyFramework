
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{

    matrix wvp;

    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    Out.Position = mul(In.Position, wvp);
    Out.Diffuse = In.Diffuse * Material.Diffuse;

    float width = 1.0f / (float) Columns;
    float height = 1.0f / (float) Rows;

    int x = Frame % Columns;
    int y = Frame / Columns;
   

    float offsetX = x * width;
    float offsetY = y * height;
   

    Out.TexCoord.x = In.TexCoord.x * width + offsetX;
    Out.TexCoord.y = In.TexCoord.y * height + offsetY;
   
}

