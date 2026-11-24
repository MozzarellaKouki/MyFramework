#pragma once
#include <DirectXMath.h>
#include "mathUtils.h"

//--------------------------------------------------------------
// Transform（位置・回転・スケール）
//
// コンポーネントではなく、SpatialObject が本体の性質として持つ。
// 取得は object->GetTransform()（検索なし）。
//--------------------------------------------------------------
class Transform
{
private:
    Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
    Vector3 m_Rotation{ 0.0f, 0.0f, 0.0f }; // ラジアン（Pitch, Yaw, Roll）
    Vector3 m_Scale{ 1.0f, 1.0f, 1.0f };

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

    DirectX::XMMATRIX GetWorldMatrix() const;
};
