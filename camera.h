#pragma once
#include "main.h"
#include "gameObject.h"
#include "mathUtils.h" 
#include "transformComponent.h"

class Camera : public GameObject
{
private:
    TransformComponent* m_Transform;
    Vector3 m_Target;
    Vector3 m_Up;

    XMMATRIX m_ViewMatrix{};
    XMMATRIX m_ProjectionMatrix{};
    XMMATRIX m_BillboardMatrix{};
    XMMATRIX m_ViewProjectionMatrix{};

    float m_Speed = 0.1f;

    GameObject* m_FollowTarget{};

public:
    void Init();
    void Update();
    void Apply();   //Renderer‚É“n‚·—p

    void UpdateFollowTarget();





    void SetTarget(GameObject* target){m_FollowTarget = target; }
    void SetView(Vector3 view) { m_Target = view; }

    const XMMATRIX& GetViewMatrix() const { return m_ViewMatrix; }
    const XMMATRIX& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const XMMATRIX& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    const XMMATRIX& GetBillboardMatrix() const { return m_BillboardMatrix; }
    

    TransformComponent* GetTransform() const {return m_Transform; }

    Vector3 GetForward() const;

  


};