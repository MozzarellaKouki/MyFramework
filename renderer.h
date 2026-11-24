#pragma once
#include "main.h"



//===============================
// シェーダー受け渡し用構造体
//===============================

struct VERTEX_3D
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT2 TexCoord;
};



struct MATERIAL
{
	DirectX::XMFLOAT4	Ambient;
	DirectX::XMFLOAT4	Diffuse;
	DirectX::XMFLOAT4	Specular;
	DirectX::XMFLOAT4	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2];
};



struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	DirectX::XMFLOAT4	Direction;
	DirectX::XMFLOAT4	Diffuse;
	DirectX::XMFLOAT4	Ambient;
};


struct SPRITE_ANIMATION
{
	int Frame;
	int Columns;
	int Rows;
	int Dummy;
};


struct POSTERIZE
{
	float Levels;
	float Dummy[3];
};

struct GRAYSCALE
{
	float Intensity;
	float Dummy[3];
};

//===============================
// ブレンドステートクラス
//===============================
enum class BlendMode
{
	Opaque,
	Alpha,
	Additive
};


class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;


	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*            m_SpriteAnimationBuffer;
	static ID3D11Buffer*            m_PosterizeBuffer;
	static ID3D11Buffer*            m_GrayscaleBuffer;




	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11BlendState*		m_BlendState;
	static ID3D11BlendState*		m_BlendStateATC;
	static ID3D11BlendState*        m_BlendStateAdditive;




public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetBlendMode(BlendMode mode);
	static void SetATCEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetProjectionMatrix3D();
	static void SetWorldMatrix(DirectX::XMMATRIX WorldMatrix);
	static void SetViewMatrix(DirectX::XMMATRIX ViewMatrix);
	static void SetProjectionMatrix(DirectX::XMMATRIX ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetSpriteAnimation(SPRITE_ANIMATION animation);
	static void SetPosterizeLevels(float levels);
	static void SetBackBufferRenderTarget();
	static void SetGrayscaleIntensity(float intensity);

	static void Set2D();
	static void Set3D();

	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }
	static ID3D11DepthStencilView* GetDepthStencilView( void ){ return m_DepthStencilView; }


	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);


};
