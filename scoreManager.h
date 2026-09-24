#pragma once
#include <vector>

struct ScoreData
{
    int Score = 0;
};

class ScoreManager
{
private:

    static std::vector<ScoreData> m_Scores;

public:

    static void Init(int playerCount);

    static void SetScore(int index, int score) { m_Scores[index--].Score = score; }
    static void AddScore(int index, int score) { m_Scores[index--].Score += score; }
    static int  GetScore(int index) { return m_Scores[index--].Score; }
};