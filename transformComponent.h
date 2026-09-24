#pragma once
#include <DirectXMath.h>
#include "mathUtils.h"
#include "component.h"
using namespace DirectX;

class TransformComponent : public Component
{
private:
    Vector3 m_Position{};
    Vector3 m_Rotation{};
    Vector3 m_Scale{};

public:

    void SetPosition(Vector3 pos) { m_Position = pos; }
    void AddPosition(Vector3 pos) { m_Position += pos; }

    void SetRotation(Vector3 rot) { m_Rotation = rot; }
    void AddRotation(Vector3 rot) { m_Rotation += rot; }

    void SetScale(Vector3 sca) { m_Scale = sca; }
    void AddScale(Vector3 sca) { m_Scale += sca; }

    Vector3 GetPosition() const { return m_Position; }
    Vector3 GetRotation() const { return m_Rotation; }
    Vector3 GetScale() const { return m_Scale; }


    XMMATRIX GetWorldMatrix() const;
};