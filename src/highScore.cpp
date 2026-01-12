#include "highScore.h"
#include <fstream>
#include <vector>
#include <algorithm>

// File se scores load karna
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

    // Agar file khali hai to default scores bhar do
    if (scores.empty())
    {
        scores = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }
    std::sort(scores.rbegin(), scores.rend()); // Baray se chota sort
    return scores;
}

// Naya score save karna
void HighScoreManager::SaveScore(int newScore)
{
    std::vector<int> scores = LoadHighScores();
    scores.push_back(newScore);                // Naya score dalo
    std::sort(scores.rbegin(), scores.rend()); // Sort karo

    // Sirf top 10 rakho
    if (scores.size() > 10)
    {
        scores.resize(10);
    }

    // File mein wapas likho (Overwrite)
    std::ofstream file("highscores.txt");
    for (int s : scores)
    {
        file << s << "\n";
    }
    file.close();
}