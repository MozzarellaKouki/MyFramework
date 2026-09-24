#pragma once

#include "component.h"
#include "audio.h"
#include "mathUtils.h"

class AudioComponent : public Component
{
private:
    Audio m_Audio;

    float m_Volume = 1.0f;
    float m_MaxDistance = 1000.0f;

public:
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;

    void Load(const char* fileName);
    void Play(bool loop = false);
    void Stop();

    void SetVolume(float volume);
    void SetMaxDistance(float distance);

    void SetDistanceVolume(float volume);

    Vector3 GetPosition() const;
    float GetMaxDistance() const;
};