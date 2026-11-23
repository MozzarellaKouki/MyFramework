#pragma once

#include "spatialObject.h"
#include "transform.h"
#include "polygon3D.h"

class Camera;

class SkyPlate : public SpatialObject
{
private:
    Camera* m_Camera = nullptr;

    Polygon3D m_Polygon;

public:
    void Init() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Uninit() override;

    void SetCamera(Camera* camera);
};
