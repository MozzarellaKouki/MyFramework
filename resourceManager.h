#pragma once
#include "resourcePool.h"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

//--------------------------------------------------------------
// ResourceLoader<T>
//
// 型ごとの「ファイルからの読み込み方」。新しいリソースの種類を増やすときは
// ここに特殊化を1つ足し、resourceLoaders.cpp に Load を書くだけでよい。
//--------------------------------------------------------------
template<typename T>
struct ResourceLoader; //特殊化のみ使う

class Mesh;
class Texture;

template<>
struct ResourceLoader<Mesh>
{
    static constexpr const char* Name = "Mesh";
    static std::unique_ptr<Mesh> Load(const std::filesystem::path& path);
};

template<>
struct ResourceLoader<Texture>
{
    static constexpr const char* Name = "Texture";
    static std::unique_ptr<Texture> Load(const std::filesystem::path& path);
};

//--------------------------------------------------------------
// ResourceManager
//
// テクスチャもメッシュも同じ書き方で読み込み・参照・解放する窓口。
//
//   Handle<Mesh> h = ResourceManager::Load<Mesh>("Asset/model/player.obj");
//   Mesh* mesh = ResourceManager::Get(h);   //解放済みなら nullptr
//
// ・同じパスは1回だけ読み込まれ、2回目以降は同じハンドルが返る
// ・Scene スコープはシーン切り替え時に SceneManager が解放する
// ・Get で取り出したポインタは「その場で使うだけ」にし、保存するならハンドルを保存する
//
// ※ Get するファイルでは、そのリソースの型のヘッダー（mesh.h など）を include すること
//--------------------------------------------------------------
class ResourceManager
{
public:

    //ファイルから読み込む（読み込み済みなら同じハンドルを返す）
    template<typename T>
    static Handle<T> Load(const std::filesystem::path& path,
        ResourceScope scope = ResourceScope::Scene)
    {
        ResourcePool<T>& pool = Pool<T>();
        const std::wstring key = MakeKey(path);

        Handle<T> handle = pool.Find(key);
        if (handle.IsValid())
        {
            if (scope == ResourceScope::Global)
                pool.Globalize(handle);
            return handle;
        }

        std::unique_ptr<T> resource = ResourceLoader<T>::Load(path);
        if (!resource)
        {
            ReportLoadFailure(ResourceLoader<T>::Name, path);
            return {};
        }

        return pool.Add(key, std::move(resource), scope);
    }

    //プログラムで作ったリソースを登録する
    //name を付ければ共有できる（例: L"builtin:cube"）。空なら登録した本人だけが使う
    template<typename T>
    static Handle<T> Register(const std::wstring& name, std::unique_ptr<T> resource,
        ResourceScope scope = ResourceScope::Scene)
    {
        return Pool<T>().Add(name, std::move(resource), scope);
    }

    //ハンドルから実体を取り出す。解放済み・無効なら nullptr
    template<typename T>
    static T* Get(Handle<T> handle)
    {
        return Pool<T>().Get(handle);
    }

    template<typename T>
    static bool IsAlive(Handle<T> handle)
    {
        return Pool<T>().IsAlive(handle);
    }

    //1つだけ解放
    template<typename T>
    static void Release(Handle<T> handle)
    {
        Pool<T>().Release(handle);
    }

    //指定スコープのリソースを全種類まとめて解放
    static void ReleaseScope(ResourceScope scope);

    //全リソースを解放（アプリ終了時）
    static void ReleaseAll();

    //種類ごとの読み込み数を出力ウィンドウに表示
    static void PrintDebugInfo();

private:

    //型ごとのプール（初めて使われた時に作られ、一覧に登録される）
    template<typename T>
    static ResourcePool<T>& Pool()
    {
        static ResourcePool<T> pool(ResourceLoader<T>::Name);
        static const bool registered = (AllPools().push_back(&pool), true);
        (void)registered;
        return pool;
    }

    static std::vector<IResourcePool*>& AllPools();

    //パスの表記ゆれ（大文字小文字、\ と /）をなくしてキーにする
    static std::wstring MakeKey(const std::filesystem::path& path);

    static void ReportLoadFailure(const char* typeName, const std::filesystem::path& path);
};
