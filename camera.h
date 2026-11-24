#pragma once
#include "main.h"
#include "spatialObject.h"
#include "mathUtils.h" 
#include "transform.h"

class Camera : public SpatialObject
{
private:
    Vector3 m_Target;
    Vector3 m_Up;

    XMMATRIX m_ViewMatrix{};
    XMMATRIX m_ProjectionMatrix{};
    XMMATRIX m_BillboardMatrix{};
    XMMATRIX m_ViewProjectionMatrix{};

    float m_Speed = 0.1f;

    SpatialObject* m_FollowTarget{};

public:
    void Init();
    void Update();
    void Apply();   //Rendererに渡す用

    void UpdateFollowTarget();





    void SetTarget(SpatialObject* target){m_FollowTarget = target; }
    void SetView(Vector3 view) { m_Target = view; }

    const XMMATRIX& GetViewMatrix() const { return m_ViewMatrix; }
    const XMMATRIX& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const XMMATRIX& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    const XMMATRIX& GetBillboardMatrix() const { return m_BillboardMatrix; }
    


    Vector3 GetForward() const;

  


};