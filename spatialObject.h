#pragma once
#include "gameObject.h"
#include "transform.h"

//--------------------------------------------------------------
// SpatialObject（空間に存在するオブジェクト）
//
// GameObject      : 位置を持たない（Spawner、BGM、ルール管理など）
// SpatialObject   : 位置・回転・スケールを本体の性質として持つ（プレイヤー、敵、地形、カメラ、空など）
//
// 「何ができるか」はこれまで通りコンポーネントで足す。
// 継承はこの1段だけにして、ここから深く派生させないこと。
//--------------------------------------------------------------
class SpatialObject : public GameObject
{
public:

    Transform* GetTransform() { return &m_Transform; }
    const Transform* GetTransform() const { return &m_Transform; }

private:

    Transform m_Transform;
};
