#pragma once
#include<map>
#include<vector>
#include "position.h"
#include "colors.h"

class Block
{
public:
    int id;
    int rowOffset;
    int colOffset;
    Block();
    void Move(int rows,int cols);
    std::vector<Position> getCellPositions();
    void Draw();
    std::map<int, std::vector<Position>> cells;

private:
    int cellSize;
    int rotationState;
    std::vector<Color> colors;
};


