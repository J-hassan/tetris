#include <raylib.h>
#include "grid.h"

int main() 
{
    const Color darkBlue = {44, 44, 127, 255}; // this method is given by the raylib library for defining colors 
        
    InitWindow(300, 600, "Tetris By Developers"); // initalizing a window having width of 300 and height of 600 and title 
    SetTargetFPS(60); // setting fps so that it runs with same speed no every device

    Grid grid = Grid(); // creating a grid object 
    grid.print(); // printing in terminal to see if rows and cols are printed correctly
    
    while (WindowShouldClose() == false) // this checks if close button or escape button is pressed and window is closed
    {
        
        BeginDrawing();

            ClearBackground(darkBlue); // making the background darkBlue - as a default background color
            grid.Draw(); // drawing the grid using rectangles

        EndDrawing();
    }
    
    CloseWindow();
}