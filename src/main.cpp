#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>
#include "highScore.h"

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

// Handle Menu Input
enum GameState
{
    MENU,
    PLAYING,
    HIGHSCORE,
    HELP,
    PAUSED
};
GameState currentState = MENU;
void HandleMenuInput(Game &game)
{
    if (IsKeyPressed(KEY_ONE))
    {
        if (currentState == MENU || currentState == PAUSED || game.gameOver)
        {
            game.Reset();
            currentState = PLAYING;
        }
    }
    if (IsKeyPressed(KEY_TWO))
        currentState = HIGHSCORE;
    if (IsKeyPressed(KEY_THREE))
        currentState = HELP;
    if (IsKeyPressed(KEY_FOUR))
        CloseWindow();
    if (IsKeyPressed(KEY_FIVE) && currentState == PAUSED)
        currentState = PLAYING;
    if (IsKeyPressed(KEY_P) && currentState == PLAYING)
        currentState = PAUSED;
}

int main()
{
    InitWindow(500, 620, "raylib Tetris");
    SetTargetFPS(60);

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Game game = Game();

    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);

        // 1. Logic aur Input Handling
        HandleMenuInput(game);

        if (currentState == PLAYING && !game.gameOver)
        {
            game.HandleInput();
            if (EventTriggered(0.2))
            {
                game.MoveBlockDown();
            }
        }

        // 2. Drawing Section (Hamesha BeginDrawing se shuru karein)
        BeginDrawing();
        ClearBackground(darkBlue);

        if (currentState == MENU)
        {
            DrawText("TETRIS MENU", 130, 100, 40, WHITE);
            DrawText("1. Start New Game", 150, 200, 20, LIGHTGRAY);
            DrawText("2. High Scores", 150, 250, 20, LIGHTGRAY);
            DrawText("3. Help", 150, 300, 20, LIGHTGRAY);
            DrawText("4. Exit", 150, 350, 20, LIGHTGRAY);
        }
        else if (currentState == PLAYING || currentState == PAUSED)
        {
            // UI Elements
            DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
            DrawTextEx(font, "Next", {370, 175}, 38, 2, WHITE);
            DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);

            char scoreText[10];
            sprintf(scoreText, "%d", game.score);
            Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
            DrawTextEx(font, scoreText, {320 + (170 - textSize.x) / 2, 65}, 38, 2, WHITE);
            DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightBlue);

            // Game Draw
            game.Draw();

            if (game.gameOver)
            {
                HighScoreManager::SaveScore(game.score);
                DrawRectangle(0, 0, 500, 620, Fade(BLACK, 0.7f));
                DrawTextEx(font, "GAME OVER", {130, 250}, 50, 2, RED);
                DrawText("Press 1 to Restart", 160, 320, 20, WHITE);
            }
            else if (currentState == PAUSED)
            {
                DrawRectangle(0, 0, 500, 620, Fade(BLACK, 0.5f));
                DrawText("PAUSED", 180, 150, 40, WHITE);
                DrawText("1. Restart", 180, 250, 20, LIGHTGRAY);
                DrawText("5. Continue", 180, 300, 20, LIGHTGRAY);
                DrawText("4. Exit", 180, 350, 20, LIGHTGRAY);
            }
        }
        else if (currentState == HIGHSCORE)
        {
            DrawText("TOP 10 SCORES", 150, 100, 30, YELLOW);
            std::vector<int> topScores = HighScoreManager::LoadHighScores();
            int size = topScores.size();
            for (int i = 0; i < size; i++)
            {
                DrawText(TextFormat("%d. %d", i + 1, topScores[i]), 180, 150 + (i * 40), 25, WHITE);
            }
            DrawText("Press 'M' for Menu", 150, 550, 20, LIGHTGRAY);
            if (IsKeyPressed(KEY_M))
                currentState = MENU;
        }
        else if (currentState == HELP)
        {
            DrawText("HOW TO PLAY", 150, 100, 30, WHITE);
            DrawText("Arrows: Move & Rotate", 120, 200, 20, LIGHTGRAY);
            DrawText("P: Pause & Resume", 120, 250, 20, LIGHTGRAY);
            DrawText("Press 'M' for Menu", 150, 550, 20, LIGHTGRAY);
            if (IsKeyPressed(KEY_M))
                currentState = MENU;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}