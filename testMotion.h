#pragma once
#include "GameObject.h"
#include "polygon2D.h"
#include "mathUtils.h"

class Polygon2D;
class Vector3;


enum class State
{
    None = 0,

    UniformVelocity,      //“™‘¬‰^“®
    UniformAcceleration,  //“§‰ß‘¬“x‰^“®
    FreeFall,             //©—R—‰º
    VerticalProjectile,   //‚’¼“Š‚°ã‚°
    HorizontalProjectile, //…•½“ŠË
    ObliqueProjectile     //Î•û“ŠË

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
    float m_Grabity;         //g
    float m_InitialVelocity; //v
    float m_Degree;          //ƒÆ


public:
    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Uninit() override;

    void Input();


    void RoopCheck();

};
