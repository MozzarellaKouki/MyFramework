#pragma once
#include "mathUtils.h"
#include "component.h"
#include "particle.h"
#include "particleTemper.h"
#include "polygon3D.h"
#include "sprite3DComponent.h"
#include "transformComponent.h"

#include <vector>

class ParticleEmitterComponent : public Component
{
private:

    static std::vector<ParticleEmitterComponent*> s_TransparentEmitters;
    void RegisterTransparentEmitter();
    void UnregisterTransparentEmitter();

    TransformComponent* m_Transform;
    std::vector<Particle> m_Particles;
    ParticleTemper m_Temper;
    BillboardType m_BillboardType = BillboardType::Full; 

    Polygon3D m_Quad;
    ID3D11ShaderResourceView* m_Texture{};
    float m_EmitTimer = 0.0f;

    bool m_Playing = false;
    float m_PlayTimer = 0.0f;

public:

    void SetBillboardType(BillboardType type)
    {
        m_BillboardType = type;
    }

    void Init() override;
    void Configure(const ParticleTemper& temper);
    void SetParticle(Particle& particle, Vector3 pos);

    void Uninit() override;

    void Update(float deltaTime) override;
    void UpdateMovement(Particle& particle, float deltaTime);
    void UpdateScale(Particle& particle, float deltaTime);
    void UpdateColor(Particle& particle, float deltaTime);
    void UpdateRotation(Particle& particle, float deltaTime);
    void UpdateLife(Particle& particle, float deltaTime);

    static void SubmitTransparentParticles();
    void RenderParticle(Particle& p);

    void Emit(int count, Vector3 pos);

  
    void Play()            { m_Playing = true;  m_PlayTimer = -1.0f; }
    void Play(float timer) { m_Playing = true;  m_PlayTimer = timer; };
    void Stop() { m_Playing = false; }



};

