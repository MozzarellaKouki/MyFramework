#include "main.h"
#include "audioComponent.h"
#include "transform.h"
#include "spatialObject.h"
#include "audioManager.h"

void AudioComponent::Init()
{
    //SpatialObject なら位置付きの音、そうでなければ位置なし（原点扱い）
    if (SpatialObject* owner = FindSpatialOwner())
        m_Transform = owner->GetTransform();

    AudioManager::Register(this);
}

void AudioComponent::Update(float dt)
{
}

void AudioComponent::Uninit()
{
    AudioManager::Unregister(this);
    m_Audio.Uninit();
}

void AudioComponent::Load(const char* fileName)
{
    m_Audio.Load(fileName);
}

void AudioComponent::Play(bool loop)
{
    m_Audio.Play(loop);
}

void AudioComponent::Stop()
{
    m_Audio.Stop();
}

void AudioComponent::SetVolume(float volume)
{
    m_Volume = volume;
    m_Audio.SetVolume(m_Volume);
}

void AudioComponent::SetMaxDistance(float distance)
{
    m_MaxDistance = distance;
}

void AudioComponent::SetDistanceVolume(float volume)
{
    m_Audio.SetVolume(m_Volume * volume);
}

Vector3 AudioComponent::GetPosition() const
{
    // 位置を持たないオブジェクト（BGM など）は原点扱い
    return m_Transform ? m_Transform->GetPosition() : Vector3(0.0f, 0.0f, 0.0f);
}

float AudioComponent::GetMaxDistance() const
{
    return m_MaxDistance;
}