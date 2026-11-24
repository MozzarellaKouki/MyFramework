#pragma once
#include <d3d11.h>
#include <string>

//--------------------------------------------------------------
// TextureManager（互換用）
//
// 中身は ResourceManager。読み込んだテクスチャは Global（アプリ終了まで常駐）になる。
// 既存コードがテクスチャのポインタを保持しているため、移行が済むまでは常駐させている。
//
// 新しいコードでは ResourceManager::Load<Texture>() とハンドルを使うこと。
//--------------------------------------------------------------
class TextureManager
{
public:
    static ID3D11ShaderResourceView* Load(const std::wstring& fileName);
};
