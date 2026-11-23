#pragma once

class ColliderComponent;

//--------------------------------------------------------------
// 当たり判定マネージャー
//
// 登録された全コライダーを毎フレーム総当たりで判定し、
// 衝突の開始 / 継続 / 終了を各 GameObject と Component に通知する。
//
//   OnCollisionEnter : 衝突し始めたフレーム
//   OnCollisionStay  : 衝突し続けている間（毎フレーム）
//   OnCollisionExit  : 離れたフレーム（片方が Destroy された時も呼ばれる）
//
// 現状は判定のみで、押し戻しなどの物理応答は行わない。
// 総当たり O(n^2) なので、数百個規模になったら空間分割を検討する。
//--------------------------------------------------------------
class CollisionManager
{
public:

    // ColliderComponent が自動で呼ぶ
    static void Register(ColliderComponent* collider);
    static void Unregister(ColliderComponent* collider);

    // 判定と通知（GameObjectManager::Update から呼ばれる）
    static void Update();

    // 全登録と衝突状態をリセット
    static void Clear();

    // 2つのコライダーが重なっているか（レイヤーは見ない）
    static bool Intersects(const ColliderComponent& a, const ColliderComponent& b);

    // 登録数・衝突中のペア数を出力ウィンドウに表示
    static void PrintDebugInfo();
};
