#pragma once

#include "component.h"

class HeightFieldData;
class TransformComponent;

//ワールド空間のX/Z座標を使用して、レンダリングされた高さフィールドを参照します。
class HeightFieldSurfaceComponent : public Component
{
private:
    const HeightFieldData* m_HeightField = nullptr;
    TransformComponent* m_Transform = nullptr;

public:
    void Init() override;
    void SetHeightField(const HeightFieldData* heightField)
    {
        m_HeightField = heightField;
    }

    bool TryGetHeight(float worldX, float worldZ, float& worldHeight) const;
};
