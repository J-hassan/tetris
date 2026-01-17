#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>
#include "highScore.h"

double lastUpdateTime = 0;
double interval = 0.5; // Default interval of 0.5 seconds

bool EventTriggered()
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
    PAUSED,
    MODE
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
        currentState = MODE;
    if (IsKeyPressed(KEY_FIVE))
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
            if (EventTriggered())
            {
                game.MoveBlockDown();
            }

            // level and timer update
            game.gameTimer += GetFrameTime();
            if (game.gameTimer >= 5.0f)
            {
                game.gameTimer = 0.0f;
                if (game.level == 16)
                {
                    game.level = 16;
                }
                else
                {
                    game.level++;
                }
                if (game.level % 2 == 0 && interval > 0.2)
                {
                    interval -= 0.05;
                    if (interval < 0.2)
                    {
                        interval = 0.2;
                    }
                }
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
            DrawText("4. Difficulty Level", 150, 350, 20, LIGHTGRAY);
            DrawText("5. Exit", 150, 400, 20, LIGHTGRAY);
        }
        else if (currentState == PLAYING || currentState == PAUSED)
        {
            // UI Elements
            DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
            DrawTextEx(font, "Next", {370, 175}, 38, 2, WHITE);
            DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);

            DrawTextEx(font, "Level", {330, 120}, 38, 2, WHITE);
            DrawTextEx(font, TextFormat("%d", game.level), {450, 120}, 38, 2, YELLOW);

            char scoreText[10];
            sprintf(scoreText, "%d", game.score);
            Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
            DrawTextEx(font, scoreText, {320 + (170 - textSize.x) / 2, 65}, 38, 2, WHITE);
            DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightBlue);

            // Game Draw
            game.Draw();

            if (game.gameOver)
            {
                if (!game.scoreSaved)
                {
                    HighScoreManager::SaveScore(game.score);
                    game.scoreSaved = true;
                }
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
        else if (currentState == MODE)
        {
            DrawText("Select Difficulty Level", 150, 100, 30, WHITE);
            DrawText("B: Beginner", 120, 200, 20, LIGHTGRAY);
            DrawText("A: Advance", 120, 250, 20, LIGHTGRAY);
            DrawText("Press 'M' for Menu", 150, 350, 20, LIGHTGRAY);
            if (IsKeyPressed(KEY_M))
            {
                currentState = MENU;
            }
            if (IsKeyPressed(KEY_B))
            {
                game.difficultyLevel = 1;
            }
            if(IsKeyPressed(KEY_A))
            {
                game.difficultyLevel = 2;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}