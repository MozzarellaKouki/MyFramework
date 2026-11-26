#pragma once
#include "main.h"
#include "renderer.h"

//--------------------------------------------------------------
// FullscreenQuad
//
// 画面全体に SRV を1枚描くだけのポリゴン。ポストエフェクト専用。
//
//   ゲームの画像（ResourceManager のテクスチャ） → Polygon2D（ハンドルで持つ）
//   描画途中の結果（RenderTexture の SRV）       → FullscreenQuad（描く間だけ借りる）
//
// ・SRV は保存せず、Draw の引数で毎回受け取る（毎フレーム変わる借り物なので）
// ・Draw の後はスロット0を空にする。次のパスで同じテクスチャを描き込み先にしても
//   「読み込み中のテクスチャに書き込む」衝突が起きない
// ・位置、サイズ、UV は常に画面全体なので、変更する手段は用意しない
//
//   m_Quad.Init("Shader//grayScalePS.cso");
//   destination.SetRenderTarget();
//   m_Quad.Draw(source);
//--------------------------------------------------------------
class FullscreenQuad
{
public:

    void Init(const char* pixelShaderFile = "Shader//unlitTexturePS.cso");
    void Uninit();

    void Draw(ID3D11ShaderResourceView* source,
        BlendMode blend = BlendMode::Opaque,
        const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

private:

    ID3D11Buffer* m_VertexBuffer = nullptr;
    ID3D11InputLayout* m_VertexLayout = nullptr;
    ID3D11VertexShader* m_VertexShader = nullptr;
    ID3D11PixelShader* m_PixelShader = nullptr;
};
