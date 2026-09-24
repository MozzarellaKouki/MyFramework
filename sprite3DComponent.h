#pragma once

#include "component.h"
#include "polygon3D.h"
#include "transformComponent.h"
#include "spriteAnimationComponent.h"

#include <vector>

enum class BillboardType
{
    None,
    Full,
    Horizontal,
    YAxis
};

class Sprite3DComponent : public Component
{
private:
    Polygon3D* m_Sprite{};
    TransformComponent* m_Transform{};

    BillboardType m_BillboardType = BillboardType::None;

    bool m_SortByDistance = false;
    bool m_Visible = true;

    static std::vector<Sprite3DComponent*> s_TransparentSprites;

    void RegisterTransparentSprite();
    void UnregisterTransparentSprite();

public:

    ~Sprite3DComponent() override
    {
        SetSortByDistance(false);
        delete m_Sprite;
    }

    void Init() override;
    void Draw() override;

    void ApplyAnimation();
    void ApplyBillboard(TransformComponent* transform);

    void SetSprite(const wchar_t* path);
    void SetBillboard(BillboardType type);
    void SetSortByDistance(bool enable);
    void SetVisible(bool visible) {  m_Visible = visible; }
    bool IsVisible() const { return m_Visible; }

  

    static void SubmitTransparentSprites();
    void RenderTransparent();

    Vector3 GetPosition() const;
    Polygon3D* GetPolygon();
    BillboardType GetBillboard() const;
};