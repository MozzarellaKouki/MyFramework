#include "main.h"
#include "audioComponent.h"
#include "transformComponent.h"
#include "gameObject.h"
#include "audioManager.h"

void AudioComponent::Init()
{
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
    return GetOwner()
        ->GetComponent<TransformComponent>()
        ->GetPosition();
}

float AudioComponent::GetMaxDistance() const
{
    return m_MaxDistance;
}