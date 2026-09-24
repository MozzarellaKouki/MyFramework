#pragma once

#include "mathUtils.h"
#include "audio.h"


enum class SEType
{
   Get,
   Enemy,
   Cursor,

    Count
};

class AudioComponent;



class AudioManager
{
private:
    static Vector3 m_ListenerPosition;
    static std::vector<AudioComponent*> m_AudioComponents;

    static constexpr int BGM_COUNT = 1;
    static Audio m_BGM[BGM_COUNT];
    static int m_CurrentBGM;

    static constexpr int SE_SLOT_COUNT = 16;
    static Audio m_SE[static_cast<int>(SEType::Count)][SE_SLOT_COUNT];



public:
    static void Init();
    static void Uninit();
    static void Update();

    static void SetListenerPosition(Vector3 position);

    static void PlayBGM(int index, const char* fileName);
    static void StopBGM();
    static void SetBGMVolume(float volume);


    static void Register(AudioComponent* audio);
    static void Unregister(AudioComponent* audio);

    static void LoadSE(SEType type, const char* fileName);
    static void PlaySE(SEType type);



 
};