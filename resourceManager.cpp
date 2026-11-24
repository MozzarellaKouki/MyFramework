#include "resourceManager.h"
#include "debugUtils.h"

#include <cwctype>

std::vector<IResourcePool*>& ResourceManager::AllPools()
{
    static std::vector<IResourcePool*> pools;
    return pools;
}

void ResourceManager::ReleaseScope(ResourceScope scope)
{
    for (IResourcePool* pool : AllPools())
        pool->ReleaseScope(scope);
}

void ResourceManager::ReleaseAll()
{
    for (IResourcePool* pool : AllPools())
        pool->ReleaseAll();
}

void ResourceManager::PrintDebugInfo()
{
    DebugLog("---- Resources ----\n");

    for (const IResourcePool* pool : AllPools())
    {
        const std::string line = std::string("  ") + pool->GetTypeName() +
            ": " + std::to_string(pool->GetAliveCount()) +
            " (Global " + std::to_string(pool->GetAliveCount(ResourceScope::Global)) +
            " / Scene " + std::to_string(pool->GetAliveCount(ResourceScope::Scene)) + ")\n";
        DebugLog(line.c_str());
    }
}

std::wstring ResourceManager::MakeKey(const std::filesystem::path& path)
{
    std::wstring key = path.wstring();

    for (wchar_t& c : key)
    {
        if (c == L'\\')
            c = L'/';
        else
            c = static_cast<wchar_t>(std::towlower(c));
    }

    //"Asset//texture" のような連続した / を1つにまとめる
    std::wstring result;
    result.reserve(key.size());
    for (wchar_t c : key)
    {
        if (c == L'/' && !result.empty() && result.back() == L'/')
            continue;
        result.push_back(c);
    }

    return result;
}

void ResourceManager::ReportLoadFailure(const char* typeName, const std::filesystem::path& path)
{
    const std::string message = std::string("[ResourceManager] ") + typeName + " の読み込みに失敗: ";
    DebugLog(message.c_str());
    OutputDebugStringW(path.c_str()); //日本語パスでも化けないようワイド文字で出す
    DebugLog("\n");
}
