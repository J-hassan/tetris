#include "highScore.h"
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<int> HighScoreManager::LoadHighScores()
{
    std::vector<int> scores;
    std::ifstream file("highscores.txt");
    int s;
    while (file >> s)
    {
        scores.push_back(s);
    }
    file.close();

    if (scores.empty())
    {
        scores = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }
    std::sort(scores.begin(), scores.end(),std::greater<int>()); 
    return scores;
}

void HighScoreManager::SaveScore(int newScore)
{
    std::vector<int> scores = LoadHighScores();
    scores.push_back(newScore);                
    std::sort(scores.begin(), scores.end(),std::greater<int>()); 

    if (scores.size() > 10)
    {
        scores.resize(10);
    }

    std::ofstream file("highscores.txt");
    for (int s : scores)
    {
        file << s << "\n";
    }
    file.close();
}