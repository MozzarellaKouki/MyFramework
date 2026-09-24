#include "main.h"
#include "audioManager.h"
#include "audioComponent.h"
#include "gameObjectManager.h"
#include "enemy3D.h"

Vector3 AudioManager::m_ListenerPosition{};
std::vector<AudioComponent*> AudioManager::m_AudioComponents;
Audio AudioManager::m_BGM[BGM_COUNT];
Audio AudioManager::m_SE[ static_cast<int>(SEType::Count) ][AudioManager::SE_SLOT_COUNT];
int AudioManager::m_CurrentBGM = -1;



void AudioManager::Init()
{
    m_ListenerPosition = Vector3(0.0f, 0.0f, 0.0f);

    //test
    LoadSE(SEType::Get, "Asset/Sound/Get_01.wav");
    LoadSE(SEType::Enemy, "Asset/Sound/Enemy_01.wav");
    LoadSE(SEType::Cursor, "Asset/Sound/Cursor_01.wav");

   

}

void AudioManager::Uninit()
{
    for (int i = 0; i < BGM_COUNT; i++)
    {
        m_BGM[i].Uninit();
    }

    m_AudioComponents.clear();
}


void AudioManager::Update()
{
    for (auto* audio : m_AudioComponents)
    {
        Vector3 soundPosition = audio->GetPosition();
        Vector3 difference = soundPosition - m_ListenerPosition;

        float distance = sqrt
        (
            difference.x * difference.x +
            difference.y * difference.y +
            difference.z * difference.z
        );

        float volume = 1.0f - distance / audio->GetMaxDistance();

        if (volume < 0.0f)
            volume = 0.0f;

        if (volume > 1.0f)
            volume = 1.0f;

        audio->SetDistanceVolume(volume);

    }


    //仮実装 (エネミーとの音の距離感)
    float nearestDistance = FLT_MAX;

    auto enemies =
        GameObjectManager::GetInstance().GetEnemies();

    for (auto* enemy : enemies)
    {
        auto* transform = enemy->GetComponent<TransformComponent>();

        if (transform == nullptr)
            continue;

        Vector3 enemyPosition =
            transform->GetPosition();

        Vector3 difference =
            enemyPosition - m_ListenerPosition;

        float distance = sqrt
        (
            difference.x * difference.x +
            difference.y * difference.y +
            difference.z * difference.z
        );

        if (distance < nearestDistance)
        {
            nearestDistance = distance;
        }
    }


    //BGMの音量を変更
    if (nearestDistance == FLT_MAX)
    {
        //敵がいない
        AudioManager::SetBGMVolume(1.0f);
    }
    else
    {
        float volume = nearestDistance / 200.0f;

        if (volume > 1.0f) volume = 1.0f;
        if (volume < 0.0f) volume = 0.0f;

        AudioManager::SetBGMVolume(volume);
    }
}




void AudioManager::SetListenerPosition(Vector3 position)
{
    m_ListenerPosition = position;
}



//==================================
//　BGMの再生・停止　など
//==================================

void AudioManager::PlayBGM(int index, const char* fileName)
{
    if (index < 0 || index >= BGM_COUNT) return;

    //既に流れているBGMを切っておく
    StopBGM();

    m_BGM[index].Load(fileName);
    m_BGM[index].Play(true);

    m_CurrentBGM = index;
}

void AudioManager::StopBGM()
{
    if (m_CurrentBGM < 0) return;

    m_BGM[m_CurrentBGM].Stop();
    m_CurrentBGM = -1;
}

void AudioManager::SetBGMVolume(float volume)
{
    if (m_CurrentBGM < 0) return;

    m_BGM[m_CurrentBGM].SetVolume(volume);
}


//==================================
//　登録・除外
//==================================

void AudioManager::Register(AudioComponent* audio)
{
    m_AudioComponents.push_back(audio);
}

void AudioManager::Unregister(AudioComponent* audio)
{
    //後で実装
}


//==================================
//　SEのロード・再生
//==================================

void AudioManager::LoadSE(SEType type, const char* fileName)
{
    int typeIndex = static_cast<int>(type);

    for (int i = 0; i < SE_SLOT_COUNT; i++)
    {
        m_SE[typeIndex][i].Load(fileName);
    }
}

void AudioManager::PlaySE(SEType type)
{
    int typeIndex = static_cast<int>(type);

    for (int i = 0; i < SE_SLOT_COUNT; i++)
    {
        if (!m_SE[typeIndex][i].IsPlaying())
        {
            m_SE[typeIndex][i].Play(false);
            return;
        }
    }
}

