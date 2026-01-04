#pragma once
#include<vector>
#include "raylib.h"

class Grid
{
public:
    Grid(); // consturctor - it has same name as class 
    void initialize(); // this function initialises a grid by setting all the rows and cols to value -> 0
    void print(); // just for the checking purpose
    bool isCellOutside(int row, int col);
    int grid[20][10]; // it stores the value for each cell
    void Draw(); // this function draws the grid by setting respective color to each grid
private:
    int numCols; // it defines how many cols the grid will have
    int numRows; // it defines how many rows the grid will have
    int cellSize; // it defines the size for each cell
    std::vector<Color> colors; // this vector stores all the colors and can be accessed as -> colors[index]
};

