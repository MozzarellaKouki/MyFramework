#include "collisionManager.h"
#include "collision.h"
#include "colliderComponent.h"
#include "boxColliderComponent.h"
#include "sphereColliderComponent.h"
#include "gameObject.h"
#include "debugUtils.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace
{
    using ColliderPair = std::pair<ColliderComponent*, ColliderComponent*>;

    std::vector<ColliderComponent*> s_Colliders;
    std::set<ColliderPair> s_ContactPairs; // 前フレームで衝突していたペア

    // ペアの順序を固定する（(a,b) と (b,a) を同じものとして扱う）
    ColliderPair MakePair(ColliderComponent* a, ColliderComponent* b)
    {
        return std::less<ColliderComponent*>()(a, b) ? ColliderPair(a, b) : ColliderPair(b, a);
    }

    bool IsActive(const ColliderComponent* c)
    {
        if (c == nullptr || !c->IsRegistered() || !c->IsEnabled())
            return false;

        const GameObject* owner = c->GetOwner();
        return owner != nullptr && !owner->IsDestroy();
    }

    bool LayerMatch(const ColliderComponent* a, const ColliderComponent* b)
    {
        return (a->GetLayer() & b->GetMask()) != 0 &&
               (b->GetLayer() & a->GetMask()) != 0;
    }

    //---------------- 形状ごとの判定 ----------------

    bool BoxBox(const BoxColliderComponent& a, const BoxColliderComponent& b)
    {
        const Vector3 d = a.GetCenter() - b.GetCenter();
        const Vector3 ha = a.GetHalfExtents();
        const Vector3 hb = b.GetHalfExtents();

        return std::fabs(d.x) <= ha.x + hb.x &&
               std::fabs(d.y) <= ha.y + hb.y &&
               std::fabs(d.z) <= ha.z + hb.z;
    }

    bool SphereSphere(const SphereColliderComponent& a, const SphereColliderComponent& b)
    {
        const Vector3 d = a.GetCenter() - b.GetCenter();
        const float r = a.GetWorldRadius() + b.GetWorldRadius();
        return d.x * d.x + d.y * d.y + d.z * d.z <= r * r;
    }

    bool BoxSphere(const BoxColliderComponent& box, const SphereColliderComponent& sphere)
    {
        const Vector3 c = box.GetCenter();
        const Vector3 h = box.GetHalfExtents();
        const Vector3 s = sphere.GetCenter();

        // 球の中心に最も近いボックス上の点
        const Vector3 closest(
            std::clamp(s.x, c.x - h.x, c.x + h.x),
            std::clamp(s.y, c.y - h.y, c.y + h.y),
            std::clamp(s.z, c.z - h.z, c.z + h.z));

        const Vector3 d = s - closest;
        const float r = sphere.GetWorldRadius();
        return d.x * d.x + d.y * d.y + d.z * d.z <= r * r;
    }

    //---------------- 通知 ----------------

    enum class Phase { Enter, Stay, Exit };

    void NotifyOne(ColliderComponent* self, ColliderComponent* other, Phase phase)
    {
        GameObject* owner = self->GetOwner();
        if (owner == nullptr)
            return;

        Collision collision;
        collision.other = other->GetOwner();
        collision.self = self;
        collision.otherCollider = other;

        switch (phase)
        {
        case Phase::Enter: owner->NotifyCollisionEnter(collision); break;
        case Phase::Stay:  owner->NotifyCollisionStay(collision);  break;
        case Phase::Exit:  owner->NotifyCollisionExit(collision);  break;
        }
    }

    void Notify(const ColliderPair& pair, Phase phase)
    {
        // 前の通知の中で登録解除された場合はスキップ
        if (!pair.first->IsRegistered() || !pair.second->IsRegistered())
            return;

        NotifyOne(pair.first, pair.second, phase);

        if (!pair.first->IsRegistered() || !pair.second->IsRegistered())
            return;

        NotifyOne(pair.second, pair.first, phase);
    }
}


void CollisionManager::Register(ColliderComponent* collider)
{
    if (collider == nullptr)
        return;

    if (std::find(s_Colliders.begin(), s_Colliders.end(), collider) == s_Colliders.end())
    {
        s_Colliders.push_back(collider);
    }
}

void CollisionManager::Unregister(ColliderComponent* collider)
{
    std::erase(s_Colliders, collider);

    // 破棄されたコライダーを含むペアを取り除く（ダングリング防止）
    std::vector<ColliderComponent*> partners;
    std::erase_if(s_ContactPairs, [collider, &partners](const ColliderPair& p)
    {
        if (p.first == collider)  { partners.push_back(p.second); return true; }
        if (p.second == collider) { partners.push_back(p.first);  return true; }
        return false;
    });

    // 衝突中だった相手には Exit を通知する（衝突コールバック内で Destroy された場合など）
    for (ColliderComponent* partner : partners)
    {
        if (partner->IsRegistered())
        {
            NotifyOne(partner, collider, Phase::Exit);
        }
    }
}

void CollisionManager::Clear()
{
    // シーン破棄時に呼ぶ。ここで消したペアには Exit を通知しない
    s_Colliders.clear();
    s_ContactPairs.clear();
}

bool CollisionManager::Intersects(const ColliderComponent& a, const ColliderComponent& b)
{
    const ColliderShape sa = a.GetShape();
    const ColliderShape sb = b.GetShape();

    if (sa == ColliderShape::Box && sb == ColliderShape::Box)
        return BoxBox(static_cast<const BoxColliderComponent&>(a),
                      static_cast<const BoxColliderComponent&>(b));

    if (sa == ColliderShape::Sphere && sb == ColliderShape::Sphere)
        return SphereSphere(static_cast<const SphereColliderComponent&>(a),
                            static_cast<const SphereColliderComponent&>(b));

    if (sa == ColliderShape::Box && sb == ColliderShape::Sphere)
        return BoxSphere(static_cast<const BoxColliderComponent&>(a),
                         static_cast<const SphereColliderComponent&>(b));

    if (sa == ColliderShape::Sphere && sb == ColliderShape::Box)
        return BoxSphere(static_cast<const BoxColliderComponent&>(b),
                         static_cast<const SphereColliderComponent&>(a));

    return false;
}

void CollisionManager::Update()
{
    // 通知中に登録・解除が起きても安全なように、判定はスナップショットで行う
    const std::vector<ColliderComponent*> colliders = s_Colliders;

    std::set<ColliderPair> current;

    for (size_t i = 0; i < colliders.size(); ++i)
    {
        ColliderComponent* a = colliders[i];
        if (!IsActive(a))
            continue;

        for (size_t j = i + 1; j < colliders.size(); ++j)
        {
            ColliderComponent* b = colliders[j];
            if (!IsActive(b))
                continue;

            // 同じオブジェクトのコライダー同士は判定しない
            if (a->GetOwner() == b->GetOwner())
                continue;

            if (!LayerMatch(a, b))
                continue;

            if (Intersects(*a, *b))
            {
                current.insert(MakePair(a, b));
            }
        }
    }

    // 前フレームとの比較で Enter / Stay / Exit を決める
    const std::set<ColliderPair> previous = std::move(s_ContactPairs);
    s_ContactPairs = current;

    for (const auto& pair : current)
    {
        Notify(pair, previous.count(pair) ? Phase::Stay : Phase::Enter);
    }

    for (const auto& pair : previous)
    {
        if (!current.count(pair))
        {
            Notify(pair, Phase::Exit);
        }
    }
}

void CollisionManager::PrintDebugInfo()
{
    std::string text = "[Collision] colliders: " + std::to_string(s_Colliders.size()) +
        "  contacts: " + std::to_string(s_ContactPairs.size()) + "\n";
    DebugLog(text.c_str());
}
