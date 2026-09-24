#pragma once

#include "gameObject.h"
#include "transformComponent.h"
#include "polygon3D.h"

class Camera;

class SkyPlate : public GameObject
{
private:
    TransformComponent* m_Transform = nullptr;
    Camera* m_Camera = nullptr;

    Polygon3D m_Polygon;

public:
    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Uninit() override;

    void SetCamera(Camera* camera);
};
