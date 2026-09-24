#pragma once
#include <d3d11.h>
#include <string>
#include <map>

class TextureManager
{
private:
    static std::map<std::wstring, ID3D11ShaderResourceView*> m_TextureMap;

public:
    static void Init();
    static void Uninit();
    static ID3D11ShaderResourceView* Load(const std::wstring& fileName);
};