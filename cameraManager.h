#pragma once

#include "camera.h"

class CameraManager
{
private:
    Camera* m_CurrentCamera = nullptr;


public:
    static CameraManager& GetInstance()
    {
        static CameraManager instance;
        return instance;
    }

    void SetCamera(Camera* cam)
    {
        m_CurrentCamera = cam;
    }

    Camera* GetCamera()
    {
        return m_CurrentCamera;
    }
};
