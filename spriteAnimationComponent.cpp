#include "spriteAnimationComponent.h"
#include "sprite3DComponent.h"
#include "gameObject.h"

#include "debugUtils.h"

void  SpriteAnimationComponent::Update(float dt)
{
    m_Timer += dt;

    if (m_Timer >= m_FrameTime)
    {
        m_Timer -= m_FrameTime;;
        m_CurrentFrame++;

        if (m_CurrentFrame > m_MaxFrame)
        {
            m_CurrentFrame = m_MaxFrame;
            GetOwner()->Destroy();  //‰¼
        }

        ApplyUV();

        DebugLog(
            ("Frame = " +std::to_string(m_CurrentFrame) + "\n").c_str());
    }
}


void SpriteAnimationComponent::ApplyUV()
{
    int x = m_CurrentFrame % m_Columns;
    int y = m_CurrentFrame / m_Columns;

    float uSize = 1.0f / m_Columns;
    float vSize = 1.0f / m_Rows;

    float u0 = x * uSize;
    float v0 = y * vSize;

    float u1 = u0 + uSize;
    float v1 = v0 + vSize;

    auto* sprite =
        GetOwner()->GetComponent<Sprite3DComponent>();

    if (sprite)
    {
        sprite->GetPolygon()->SetUV(u0, v0, u1, v1);
    }
}