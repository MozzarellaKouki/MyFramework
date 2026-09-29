#pragma once
#include "colliderComponent.h"
#include "transformComponent.h"

#include "mathUtils.h"

class TransformComponent;

class BoxColliderComponent : public ColliderComponent
{
private:
    Vector3 m_Size = { 1.0f, 1.0f, 1.0f };
    TransformComponent* m_Transform = nullptr;


public:
    void Init() override;



    bool CheckCollision(ColliderComponent* other) override;


    void SetSize(const Vector3& size)
    {
        m_Size = size;
    }

    Vector3 GetSize() const
    {
        return m_Size;
    }

    Vector3 GetPosition() const
    {
        return m_Transform->GetPosition();
    }

    Vector3 GetWorldSize() const
    {
        Vector3 scale = m_Transform->GetScale();

        return 
        {
            m_Size.x * scale.x,
            m_Size.y * scale.y,
            m_Size.z * scale.z
        };
    }


};