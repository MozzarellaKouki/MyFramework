#pragma once

#include <xaudio2.h>


class Audio
{
private:
    static IXAudio2* m_Xaudio;
    static IXAudio2MasteringVoice* m_MasteringVoice;

    IXAudio2SourceVoice* m_SourceVoice = nullptr;

    unsigned char* m_SoundData = nullptr;

    UINT32 m_Length = 0;
    UINT32 m_PlayLength = 0;

public:
    static void InitMaster();
    static void UninitMaster();

    void Load(const char* FileName);
    void Uninit();

    void Play(bool Loop);
    void Stop();

    void SetVolume(float volume);

    bool IsPlaying() const;
 
};

