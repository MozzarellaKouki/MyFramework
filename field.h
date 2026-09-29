#pragma once
#include "gameObject.h"
#include "renderer.h"
#include <vector>

#include "transformComponent.h"

class Field : public GameObject
{
private:
    ID3D11Buffer* m_VertexBuffer = nullptr;

    ID3D11InputLayout* m_VertexLayout = nullptr;
    ID3D11VertexShader* m_VertexShader = nullptr;
    ID3D11PixelShader* m_PixelShader = nullptr;

    ID3D11ShaderResourceView* m_Texture = nullptr;

    // 頂点の細かさ
    int m_GridSize = 1;

    
    // フィールドの大きさ
    TransformComponent* m_Transform = nullptr;

    // Wave  ===========================================================

    bool m_WaveEnabled = false;

    // 波の設定
    float m_WaveHeight = 2.0f;
    float m_WaveFrequency = 0.5f;
    float m_WaveSpeed = 1.0f;

    // 波の経過時間
    float m_WaveTime = 0.0f;

    // 頂点データ
    std::vector<VERTEX_3D> m_Vertices;

public:
    void Init() override;

    void Update(float dt) override;

    void Draw() override;

    void Uninit() override;


    // 設定
    void SetGridSize(int gridSize)
    {
        m_GridSize = gridSize;
    }

    void SetWaveEnabled(bool enabled)
    {
        m_WaveEnabled = enabled;
    }

    void SetWaveHeight(float height)
    {
        m_WaveHeight = height;
    }

    void SetWaveFrequency(float frequency)
    {
        m_WaveFrequency = frequency;
    }

    void SetWaveSpeed(float speed)
    {
        m_WaveSpeed = speed;
    }


    
    void UpdateWave(float dt);
};