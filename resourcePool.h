#pragma once
#include "resourceHandle.h"

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

//--------------------------------------------------------------
// 型を問わないプールの窓口
// ResourceManager が全プールをまとめて解放するために使う
//--------------------------------------------------------------
class IResourcePool
{
public:
    virtual ~IResourcePool() = default;

    virtual void ReleaseScope(ResourceScope scope) = 0;
    virtual void ReleaseAll() = 0;

    virtual const char* GetTypeName() const = 0;
    virtual size_t GetAliveCount() const = 0;
    virtual size_t GetAliveCount(ResourceScope scope) const = 0;
};

//--------------------------------------------------------------
// ResourcePool<T>
//
// 1種類のリソース（Mesh、Texture など）を持つ入れ物。
// ・リソースの持ち主はプール（unique_ptr）。使う側はハンドルで借りる
// ・キー（ファイルパスなど）で検索でき、同じキーは1つだけ読み込まれる
// ・解放したスロットは世代を進めて再利用する
//--------------------------------------------------------------
template<typename T>
class ResourcePool : public IResourcePool
{
public:

    explicit ResourcePool(const char* typeName) : m_TypeName(typeName) {}

    //キーで検索（なければ無効なハンドル）
    Handle<T> Find(const std::wstring& key) const
    {
        auto it = m_KeyToIndex.find(key);
        if (it == m_KeyToIndex.end())
            return {};

        return { it->second, m_Slots[it->second].generation };
    }

    //リソースを追加する。key が空なら検索できない（共有しない）リソースになる
    Handle<T> Add(const std::wstring& key, std::unique_ptr<T> resource, ResourceScope scope)
    {
        if (!resource)
            return {};

        if (!key.empty())
        {
            Handle<T> existing = Find(key);
            if (existing.IsValid())
                return existing;
        }

        uint32_t index;
        if (!m_FreeList.empty())
        {
            //空きスロットを再利用（世代は解放時に進めてある）
            index = m_FreeList.back();
            m_FreeList.pop_back();
        }
        else
        {
            index = static_cast<uint32_t>(m_Slots.size());
            m_Slots.emplace_back();
        }

        Slot& slot = m_Slots[index];
        slot.resource = std::move(resource);
        slot.key = key;
        slot.scope = scope;

        if (!key.empty())
            m_KeyToIndex[key] = index;

        return { index, slot.generation };
    }

    //ハンドルから実体を取り出す。解放済み・無効なら nullptr
    T* Get(Handle<T> handle) const
    {
        if (!IsAlive(handle))
            return nullptr;

        return m_Slots[handle.index].resource.get();
    }

    bool IsAlive(Handle<T> handle) const
    {
        return handle.index < m_Slots.size() &&
               m_Slots[handle.index].generation == handle.generation &&
               m_Slots[handle.index].resource != nullptr;
    }

    //Scene で読み込んだものを、後から Global として要求された場合に格上げする
    void PromoteToGlobal(Handle<T> handle)
    {
        if (IsAlive(handle))
            m_Slots[handle.index].scope = ResourceScope::Global;
    }

    //1つだけ解放
    void Release(Handle<T> handle)
    {
        if (IsAlive(handle))
            FreeSlot(handle.index);
    }

    void ReleaseScope(ResourceScope scope) override
    {
        for (uint32_t i = 0; i < m_Slots.size(); ++i)
        {
            if (m_Slots[i].resource && m_Slots[i].scope == scope)
                FreeSlot(i);
        }
    }

    void ReleaseAll() override
    {
        for (uint32_t i = 0; i < m_Slots.size(); ++i)
        {
            if (m_Slots[i].resource)
                FreeSlot(i);
        }
    }

    const char* GetTypeName() const override { return m_TypeName; }

    size_t GetAliveCount() const override
    {
        size_t count = 0;
        for (const auto& slot : m_Slots)
            if (slot.resource) ++count;
        return count;
    }

    size_t GetAliveCount(ResourceScope scope) const override
    {
        size_t count = 0;
        for (const auto& slot : m_Slots)
            if (slot.resource && slot.scope == scope) ++count;
        return count;
    }

private:

    struct Slot
    {
        std::unique_ptr<T> resource;
        std::wstring key;
        uint32_t generation = 1; //0 は「一度も使われていない」ハンドル用に空けておく
        ResourceScope scope = ResourceScope::Scene;
    };

    void FreeSlot(uint32_t index)
    {
        Slot& slot = m_Slots[index];

        if (!slot.key.empty())
            m_KeyToIndex.erase(slot.key);

        slot.resource.reset();
        slot.key.clear();
        ++slot.generation;          //これで古いハンドルは全部無効になる
        m_FreeList.push_back(index);
    }

    const char* m_TypeName;
    std::vector<Slot> m_Slots;
    std::vector<uint32_t> m_FreeList;
    std::unordered_map<std::wstring, uint32_t> m_KeyToIndex;
};
