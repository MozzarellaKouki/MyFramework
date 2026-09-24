#pragma once
#include "gameObject.h"
#include "transformComponent.h"
#include "meshRendererComponent.h"
#include "physicsComponent.h"
#include "particleemittercomponent.h"

#include "audio.h"

enum class Player3DState
{
	None = 0,

	Normal,


};


class Player3D : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11ShaderResourceView* m_Texture;

	TransformComponent* m_transform;
	PhysicsComponent* m_physics;
	MeshRendererComponent* m_renderer;
	Player3DState m_State;

	bool m_Boost = false;
	int m_BoostFrameCount = 0;
	ParticleEmitterComponent* m_SmokeEmitter = nullptr;


	//test
	//Audio m_TestSE;


public:
	void Init() override;
	void Update(float deltaTime) override;
	void Draw() override;
	void Uninit() override;


	void SetState(Player3DState state) { m_State = state; }
	void InputMove();

};
