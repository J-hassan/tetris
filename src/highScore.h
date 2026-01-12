#pragma once
#include<vector>
#include<algorithm>
#include<fstream>

class HighScoreManager
{
    public:
    static std::vector<int> LoadHighScores();
    static void SaveScore(int newScore);
};