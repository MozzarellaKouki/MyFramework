#include "particleEmitterComponent.h"
#include "textureManager.h"
#include "cameramanager.h"
#include "renderer.h"
#include "debugUtils.h"

#include "transformComponent.h"
#include "transparentRenderManager.h"


std::vector<ParticleEmitterComponent*> ParticleEmitterComponent::s_TransparentEmitters;


//==============================
//   Init
//==============================
void ParticleEmitterComponent::Init()
{
	m_Transform = GetOwner()->GetComponent<TransformComponent>();
	assert(m_Transform);

	m_Quad.Init();

}

void ParticleEmitterComponent::Configure(const ParticleTemper& temper) //生成後に渡す
{
	m_Temper = temper;

	//パーティクル数を確定
	m_Particles.clear();
	m_Particles.resize(m_Temper.MaxParticles);


	RegisterTransparentEmitter();
}


//==============================
//   Uninit
//==============================

void ParticleEmitterComponent::Uninit()
{
	UnregisterTransparentEmitter();

	m_Quad.Uninit();
}


//==============================
//   Update
//==============================
void ParticleEmitterComponent::Update(float deltaTime)
{
	for (auto& particle : m_Particles)
	{
		if (!particle.Alive)
			continue;

		UpdateMovement(particle, deltaTime);
		UpdateScale(particle, deltaTime);
		UpdateColor(particle, deltaTime);
		UpdateRotation(particle, deltaTime);
		UpdateLife(particle, deltaTime);

	}

	if (m_Playing)
	{
	
		//タイマー処理
		if (m_PlayTimer > 0.0f)
		{
			m_PlayTimer -= deltaTime;

			if (m_PlayTimer <= 0.0f)
			{
				Stop();
			}
		}


		//エミット処理
		if (m_Temper.EmitRate <= 0.0f) { DebugLog("!!! ParticleEmitterComponent : EmitRate <= 0 !!!"); return; }
		float interval = 1.0f / m_Temper.EmitRate;

		m_EmitTimer += deltaTime;
		while (m_EmitTimer >= interval)
		{
			Emit(1, m_Transform->GetPosition());

			m_EmitTimer -= interval;
		}

	}
	
}


void ParticleEmitterComponent::UpdateMovement(Particle& particle, float dt)
{
	particle.Velocity += particle.Acceleration * dt;
	particle.Position += particle.Velocity * dt;
}


void ParticleEmitterComponent::UpdateScale(Particle& particle, float dt)
{
	float t = 1.0f - particle.Life / particle.LifeLimit;
	particle.Scale = Lerp(particle.StartScale, particle.EndScale, t);
}


void ParticleEmitterComponent::UpdateColor(Particle& particle, float dt)
{
   
}


void ParticleEmitterComponent::UpdateRotation(Particle& particle, float dt)
{
	particle.Rotation += particle.RotationSpeed * dt;
}


void ParticleEmitterComponent::UpdateLife(Particle& particle, float dt)
{
	particle.Life -= dt;

	if (particle.Life <= 0.0f)
	{
		particle.Alive = false;
	}
}


//=================================
//           Draw
//=================================

void ParticleEmitterComponent::SubmitTransparentParticles()
{
	for (auto* emitter : s_TransparentEmitters)
	{
		for (auto& particle : emitter->m_Particles)
		{
			if (!particle.Alive)
				continue;

			TransparentRenderManager::RegisterParticle(
				emitter,
				&particle);
		}
	}
}

void ParticleEmitterComponent::RenderParticle(Particle& p)
{

	Camera* cam = CameraManager::GetInstance().GetCamera();
	if (!cam) return;

	XMMATRIX B;

	switch (m_BillboardType)
	{
	case BillboardType::Full:
		B = cam->GetBillboardMatrix();
		break;

	case BillboardType::Horizontal:
		B = XMMatrixRotationX(
			XMConvertToRadians(90.0f));
		break;

	default:
		B = XMMatrixIdentity();
		break;
	}

	XMMATRIX S = XMMatrixScaling
	(
		p.Scale,
		p.Scale,
		p.Scale
	);
	XMMATRIX T = XMMatrixTranslation
	(
		p.Position.x,
		p.Position.y,
		p.Position.z
	);
	XMMATRIX R = XMMatrixRotationZ(p.Rotation);


	Renderer::SetWorldMatrix(S * R * B * T);

	m_Quad.SetTexture(p.Texture);
	m_Quad.Draw();
}


//void ParticleEmitterComponent::Render()
//{
//	Camera* cam = CameraManager::GetInstance().GetCamera();
//	if (!cam) return;
//
//	//const XMMATRIX& B = cam->GetBillboardMatrix();
//	XMMATRIX B;
//
//	switch (m_BillboardType)
//	{
//	case BillboardType::Full:
//		B = cam->GetBillboardMatrix();
//		break;
//
//	case BillboardType::Horizontal:
//		B = XMMatrixRotationX(XMConvertToRadians(90.0f));
//		break;
//	}
//
//	for (auto& p : m_Particles)
//	{
//		if (!p.Alive)
//			continue;
//
//		XMMATRIX S = XMMatrixScaling(
//			p.Scale,
//			p.Scale,
//			p.Scale);
//
//		XMMATRIX T = XMMatrixTranslation(
//			p.Position.x,
//			p.Position.y,
//			p.Position.z);
//
//		XMMATRIX R = XMMatrixRotationZ(p.Rotation);
//
//		Renderer::SetWorldMatrix(S * R * B * T);
//
//		m_Quad.SetTexture(p.Texture);
//		m_Quad.Draw();
//	}
//}


void ParticleEmitterComponent::Emit(int count, Vector3 pos)
{
	for (auto& particle : m_Particles)
	{
		if (particle.Alive)
			continue;

		SetParticle(particle, pos);

		count--; 
		if (count <= 0) { break; };
	}
}



//=================================
//　パーティクル初期化
//=================================
void ParticleEmitterComponent::SetParticle(Particle& particle, Vector3 pos)
{
	particle.Alive = true;


	particle.Position.x = pos.x + RandomRange(m_Temper.MinPosition.x,m_Temper.MaxPosition.x);
	particle.Position.y = pos.y + RandomRange(m_Temper.MinPosition.y,m_Temper.MaxPosition.y);
	particle.Position.z = pos.z + RandomRange(m_Temper.MinPosition.z,m_Temper.MaxPosition.z);

	particle.Velocity.x = RandomRange(m_Temper.MinVelocity.x,m_Temper.MaxVelocity.x);
	particle.Velocity.y = RandomRange(m_Temper.MinVelocity.y,m_Temper.MaxVelocity.y);
	particle.Velocity.z = RandomRange(m_Temper.MinVelocity.z,m_Temper.MaxVelocity.z);

	particle.Acceleration = m_Temper.Acceleration;

	particle.Rotation = 0.0f;
	particle.RotationSpeed = RandomRange(-5, 5);

	particle.StartScale = RandomRange(m_Temper.MinStartScale, m_Temper.MaxStartScale);
	particle.EndScale = RandomRange(m_Temper.MinEndScale, m_Temper.MaxEndScale);
	particle.Scale = particle.StartScale;

	particle.Life = RandomRange(m_Temper.MinLife,m_Temper.MaxLife);
	particle.LifeLimit = particle.Life;

	if (m_Temper.TexturePath.empty()) { DebugLog("!!! ParticleEmitterComponent : TextyreePath Empty !!!"); return; }
	int index =	rand() % m_Temper.TexturePath.size();
	particle.Texture = TextureManager::Load( m_Temper.TexturePath[index] );


}


//=================================
//　透過エミッターの登録・削除
//=================================
void ParticleEmitterComponent::RegisterTransparentEmitter()
{
	s_TransparentEmitters.push_back(this);
}

void ParticleEmitterComponent::UnregisterTransparentEmitter()
{
	auto it = std::find(s_TransparentEmitters.begin(), s_TransparentEmitters.end(), this);

	if (it != s_TransparentEmitters.end())
	{
		s_TransparentEmitters.erase(it);
	}
}