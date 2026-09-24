#pragma once
#include "component.h"
#include <vector>






class GameObject
{
private:
    std::vector<Component*> m_Components;
    bool m_Destroy;

public:

    //デストラクタ
    virtual ~GameObject()
    {
        for (auto comp : m_Components)
        {
            comp->Uninit();
            delete comp;
        }
        m_Components.clear();
    }

    //----------------------------------------


 
    //コンポーネントを追加 
    template<typename T> T* AddComponent()
    {

            T* comp = new T();
            comp->SetOwner(this);
            m_Components.push_back(comp);
            comp->Init(); 
            return comp;

    }



    //コンポーネントを取得
    template<typename T> T* GetComponent()
    {
        for (auto comp : m_Components)
        {
            T* result = dynamic_cast<T*>(comp);
            if (result)
            {
                return result;
            }
        }

        return nullptr;
    }

   template<typename T> const T* GetComponent() const
    {
        for (auto comp : m_Components)
        {
            T* result = dynamic_cast<T*>(comp);
            if (result)
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

    // 削除予約
    void Destroy() { m_Destroy = true; }
    bool IsDestroy() const { return m_Destroy; }



    virtual void Init();
    virtual void Update(float deltaTime);
    virtual void Draw();
    virtual void Uninit();

};
