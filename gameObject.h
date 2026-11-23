#pragma once
#include "component.h"
#include <memory>
#include <vector>

class Transform;


//--------------------------------------------------------------
// GameObject
//
// コンポーネントは AddComponent した時点で Init される。
// コンポーネントの所有権は GameObject が持ち、Uninit / デストラクタで解放する。
//--------------------------------------------------------------
class GameObject
{
public:

    GameObject();

    virtual ~GameObject()
    {
        UninitComponents();
    }

    // コピー禁止（コンポーネントの所有者が1つであるため）
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    //----------------------------------------

    //コンポーネントを追加
    template<typename T> T* AddComponent()
    {
        auto comp = std::make_unique<T>();
        T* ptr = comp.get();
        ptr->SetOwner(this);
        m_Components.push_back(std::move(comp));
        ptr->Init();
        return ptr;
    }

    //コンポーネントを取得
    template<typename T> T* GetComponent()
    {
        for (auto& comp : m_Components)
        {
            if (T* result = dynamic_cast<T*>(comp.get()))
            {
                return result;
            }
        }

        return nullptr;
    }

    template<typename T> const T* GetComponent() const
    {
        for (const auto& comp : m_Components)
        {
            if (const T* result = dynamic_cast<const T*>(comp.get()))
            {
                return result;
            }
        }

        return nullptr;
    }

    //コンポーネントの数を取得
    size_t GetComponentCount() const
    {
        return m_Components.size();
    }

    // 位置情報（GameObject は位置を持たないので nullptr。SpatialObject が実体を返す）
    virtual Transform* GetTransform() { return nullptr; }
    virtual const Transform* GetTransform() const { return nullptr; }

    // 削除予約
    void Destroy() { m_Destroy = true; }
    bool IsDestroy() const { return m_Destroy; }


    virtual void Init();
    virtual void Update(float deltaTime);
    virtual void Draw();
    virtual void Uninit();

    // 当たり判定の通知（必要なものだけ override する）
    virtual void OnCollisionEnter(const Collision& collision) {}
    virtual void OnCollisionStay(const Collision& collision) {}
    virtual void OnCollisionExit(const Collision& collision) {}

    // CollisionManager が呼ぶ。自分の OnCollision* → 全コンポーネントの OnCollision* の順に通知
    void NotifyCollisionEnter(const Collision& collision);
    void NotifyCollisionStay(const Collision& collision);
    void NotifyCollisionExit(const Collision& collision);

protected:

    // 全コンポーネントの Uninit と解放（複数回呼んでも安全）
    void UninitComponents();

private:
    std::vector<std::unique_ptr<Component>> m_Components;
    bool m_Destroy;
};