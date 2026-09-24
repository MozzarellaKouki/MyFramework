#pragma once
#include "GameObject.h"
#include "mathUtils.h"
#include "polygon2D.h"

class Polygon2D;
class Vector3;


enum class PlayerState
{
    None = 0,

    Normal,


};


class Player : public GameObject
{
private:

    PlayerState m_State;
 


public:
    void Init() override;
    void Update() override;
    void Draw() override;
    void Uninit() override;

    void ChangeState(PlayerState state);

    void InputMove();
};
