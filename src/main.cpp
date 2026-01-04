#include <raylib.h>
#include "game.h"

int main()
{
    const Color darkBlue = {44, 44, 127, 255};

    InitWindow(300, 600, "Tetris By Developers");
    SetTargetFPS(60);

    Game game = Game();

    while (WindowShouldClose() == false)
    {

        game.handleInput();
        BeginDrawing();
        ClearBackground(darkBlue);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
}