#pragma once
#include "GameObject.h"
#include "polygon2D.h"
#include "mathUtils.h"

class Polygon2D;
class Vector3;


enum class State
{
    None = 0,

    UniformVelocity,      //等速運動
    UniformAcceleration,  //透過速度運動
    FreeFall,             //自由落下
    VerticalProjectile,   //垂直投げ上げ
    HorizontalProjectile, //水平投射
    ObliqueProjectile     //斜方投射

};


class TestMotion : public GameObject
{
private:

    Polygon2D* m_Sprite{};
    State m_State;
    Vector3 m_Velocity{};

    float m_Time;            //t
    float m_Speed;           //s
    float m_Acceleration;    //a
    float m_Gravity;         //g
    float m_InitialVelocity; //v
    float m_Degree;          //θ


public:
    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Uninit() override;

    void Input();


    void LoopCheck();

};
