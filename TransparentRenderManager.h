#pragma once

#include <vector>
#include <cstddef>

class Sprite3DComponent;
class ParticleEmitterComponent;
struct Particle;

class TransparentRenderManager
{
private:

    enum class DrawType
    {
        Sprite,
        Particle
    };

    struct DrawItem
    {
        DrawType type;

        Sprite3DComponent* sprite = nullptr;
        ParticleEmitterComponent* emitter = nullptr;
        Particle* particle = nullptr;

        float distanceSquared = 0.0f;
    };

    static std::vector<DrawItem> m_DrawItems;

public:

    static void Begin();

    static void RegisterSprite(Sprite3DComponent* sprite);
    static void RegisterParticle( ParticleEmitterComponent* emitter, Particle* particle);

    static void Draw();
};