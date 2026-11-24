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

    DirectX::XMMATRIX m_ViewMatrix{};
    DirectX::XMMATRIX m_ProjectionMatrix{};
    DirectX::XMMATRIX m_BillboardMatrix{};
    DirectX::XMMATRIX m_ViewProjectionMatrix{};

    float m_Speed = 0.1f;

    SpatialObject* m_FollowTarget{};

public:
    void Init();
    void Update();
    void Apply();   //Rendererに渡す用

    void UpdateFollowTarget();





    void SetTarget(SpatialObject* target){m_FollowTarget = target; }
    void SetView(Vector3 view) { m_Target = view; }

    const DirectX::XMMATRIX& GetViewMatrix() const { return m_ViewMatrix; }
    const DirectX::XMMATRIX& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const DirectX::XMMATRIX& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    const DirectX::XMMATRIX& GetBillboardMatrix() const { return m_BillboardMatrix; }
    


    Vector3 GetForward() const;

  


};