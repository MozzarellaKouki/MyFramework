#pragma once
#include "component.h"



class SpriteAnimationComponent : public Component
{
private:

    int m_Columns = 1; //X
    int m_Rows = 1;    //Y
    int m_MaxFrame = 1;


    float m_Timer = 0.0f;
    float m_FrameTime = 0.1f;
    int m_CurrentFrame = 0;

public:

    void Update(float deltaTime) override;

    bool IsFinished() const { return m_CurrentFrame >= m_MaxFrame; }
    void ApplyUV();


    void SetGrid(int Column, int Row) 
    {
        m_Columns = Column;
        m_Rows = Row;
        m_MaxFrame = Column * Row - 1;

        m_CurrentFrame = 0;
    }
    void SetFrameTime(float time) { m_FrameTime = time; }
    void SetMaxFrame(int max) { m_MaxFrame = max; }
    


    int GetCurrentFrame() const { return m_CurrentFrame; }
    int GetColumns() const { return m_Columns; }
    int GetRows() const { return m_Rows; }
};