#pragma once
#include <cstdint>

class SpatialObject;
class ColliderComponent;

//--------------------------------------------------------------
// 衝突情報（OnCollisionEnter / Stay / Exit に渡される）
//--------------------------------------------------------------
struct Collision
{
    SpatialObject*     other         = nullptr; // 相手のオブジェクト（コライダーの持ち主は必ず SpatialObject）
    ColliderComponent* self          = nullptr; // 自分側のコライダー
    ColliderComponent* otherCollider = nullptr; // 相手側のコライダー
};

//--------------------------------------------------------------
// 当たり判定レイヤー（ビットフラグ）
//
// 判定条件: (A.layer & B.mask) && (B.layer & A.mask)
// ゲームに合わせて自由に追加してよい（最大32個）
//--------------------------------------------------------------
namespace CollisionLayer
{
    constexpr uint32_t None    = 0;
    constexpr uint32_t Default = 1u << 0;
    constexpr uint32_t Player  = 1u << 1;
    constexpr uint32_t Enemy   = 1u << 2;
    constexpr uint32_t Item    = 1u << 3;

    constexpr uint32_t All     = 0xFFFFFFFFu;
}
