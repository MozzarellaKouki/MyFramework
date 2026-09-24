#include "gameObject.h"

#include "debugUtils.h"


void GameObject::Init()
{
    for (auto comp : m_Components)
    {
        comp->Init();
    }
}


void GameObject::Update(float deltaTime)
{
   

    for (auto comp : m_Components)
    {
        comp->Update(deltaTime);
    }
}

void GameObject::Draw()
{


    for (auto comp : m_Components)
    {
        comp->Draw();
    }

   

}

void GameObject::Uninit()
{
    for (auto comp : m_Components)
    {
        comp->Uninit();

        delete comp;
    }

    m_Components.clear();
}


