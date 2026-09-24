#include "main.h"
#include "transparentRenderManager.h"

#include "sprite3DComponent.h"
#include "particleEmitterComponent.h"
#include "particle.h"
#include "cameraManager.h"
#include "camera.h"
#include "transformComponent.h"

#include <algorithm>

std::vector<TransparentRenderManager::DrawItem>
TransparentRenderManager::m_DrawItems;

void TransparentRenderManager::Begin()
{
    m_DrawItems.clear();
}

void TransparentRenderManager::RegisterSprite(Sprite3DComponent* sprite)
{
    if (!sprite) return;

    Camera* cam = CameraManager::GetInstance().GetCamera();
    if (!cam) return;

    Vector3 camPos = cam->GetTransform()->GetPosition();
    Vector3 spritePos = sprite->GetPosition();

    float dx = spritePos.x - camPos.x;
    float dy = spritePos.y - camPos.y;
    float dz = spritePos.z - camPos.z;

    DrawItem item;

    item.type = DrawType::Sprite;
    item.sprite = sprite;

    item.distanceSquared =
        dx * dx +
        dy * dy +
        dz * dz;

    m_DrawItems.push_back(item);
}

void TransparentRenderManager::RegisterParticle( ParticleEmitterComponent* emitter, Particle* particle)
{
    if (!emitter || !particle)
        return;

    Camera* cam = CameraManager::GetInstance().GetCamera();
    if (!cam) return;

    Vector3 camPos = cam->GetTransform()->GetPosition();

    Vector3 particlePos = particle->Position;

    float dx = particlePos.x - camPos.x;
    float dy = particlePos.y - camPos.y;
    float dz = particlePos.z - camPos.z;

    DrawItem item;

    item.type = DrawType::Particle;

    item.emitter = emitter;
    item.particle = particle;

    item.distanceSquared =
        dx * dx +
        dy * dy +
        dz * dz;

    m_DrawItems.push_back(item);
}

void TransparentRenderManager::Draw()
{
    std::sort
    (
        m_DrawItems.begin(),
        m_DrawItems.end(),
        [](const DrawItem& a, const DrawItem& b)
        {
            return a.distanceSquared >
                b.distanceSquared;
        }
    );

    for (auto& item : m_DrawItems)
    {
        switch (item.type)
        {
        case DrawType::Sprite:

            item.sprite->RenderTransparent();
            break;

        case DrawType::Particle:

            item.emitter->RenderParticle(*item.particle);

            break;
        }
    }
}