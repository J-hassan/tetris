#include "grid.h"
#include <iostream>
#include "colors.h"

Grid::Grid()
{
    numRows = 20;
    numCols = 10;
    cellSize = 30;
    Initialize();
    colors = GetCellColors();
}

void Grid::Initialize()
{
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numCols; column++)
        {
            grid[row][column] = 0;
        }
    }
}

void Grid::Print()
{
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numCols; column++)
        {
            std::cout << grid[row][column] << " ";
        }
        std::cout << std::endl;
    }
}

void Grid::Draw()
{
    for (int row = 0; row < numRows; row++)
    {
        for (int column = 0; column < numCols; column++)
        {
            int cellValue = grid[row][column];
            DrawRectangle(column * cellSize + 11, row * cellSize + 11, 
                         cellSize - 1, cellSize - 1, colors[cellValue]);
        }
    }
    
    // NEW: Draw grid lines if enabled
    if (showGridLines && false) { // Set to true to enable
        for (int row = 0; row <= numRows; row++) {
            DrawLine(11, row * cellSize + 11, 
                    11 + numCols * cellSize, row * cellSize + 11, 
                    Fade(GRAY, 0.3f));
        }
        for (int col = 0; col <= numCols; col++) {
            DrawLine(col * cellSize + 11, 11, 
                    col * cellSize + 11, 11 + numRows * cellSize, 
                    Fade(GRAY, 0.3f));
        }
    }
}

void Grid::DrawWithGridLines()
{
    Draw();
    
    // Grid lines
    for (int row = 0; row <= numRows; row++) {
        DrawLine(11, row * cellSize + 11, 
                11 + numCols * cellSize, row * cellSize + 11, 
                Fade(WHITE, 0.2f));
    }
    for (int col = 0; col <= numCols; col++) {
        DrawLine(col * cellSize + 11, 11, 
                col * cellSize + 11, 11 + numRows * cellSize, 
                Fade(WHITE, 0.2f));
    }
}

bool Grid::IsCellOutside(int row, int column)
{
    if (row >= 0 && row < numRows && column >= 0 && column < numCols)
    {
        return false;
    }
    return true;
}

bool Grid::IsCellEmpty(int row, int column)
{
    if (grid[row][column] == 0)
    {
        return true;
    }
    return false;
}

int Grid::ClearFullRows()
{
    int completed = 0;
    for (int row = numRows - 1; row >= 0; row--)
    {
        if (IsRowFull(row))
        {
            ClearRow(row);
            completed++;
        }
        else if (completed > 0)
        {
            MoveRowDown(row, completed);
        }
    }
    return completed;
}

bool Grid::IsRowFull(int row)
{
    for (int column = 0; column < numCols; column++)
    {
        if (grid[row][column] == 0)
        {
            return false;
        }
    }
    return true;
}

void Grid::ClearRow(int row)
{
    for (int column = 0; column < numCols; column++)
    {
        grid[row][column] = 0;
    }
}

void Grid::MoveRowDown(int row, int numRows)
{
    for (int column = 0; column < numCols; column++)
    {
        grid[row + numRows][column] = grid[row][column];
        grid[row][column] = 0;
    }
}
