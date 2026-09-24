#include "scoreManager.h"

std::vector<ScoreData> ScoreManager::m_Scores;

void ScoreManager::Init(int playerCount)
{
    m_Scores.clear();
    m_Scores.resize(playerCount);
}
