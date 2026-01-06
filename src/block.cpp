#include "block.h"
#include<vector>

Block::Block()
{
    cellSize = 30;
    rotationState = 0;
    colors = getCellColors();
    rowOffset = 0;
    colOffset = 0;
}

void Block::Draw()
{
    std::vector<Position> tiles = getCellPositions();

    for(auto item: tiles){
        DrawRectangle(item.col * cellSize+1,item.row * cellSize+1,cellSize-1,cellSize-1,colors[id]);
    }
}

void Block::Move(int rows, int cols)
{
    rowOffset += rows;
    colOffset += cols;
}

std::vector<Position> Block::getCellPositions()
{
    std::vector<Position> tiles = cells[rotationState];
    std::vector<Position> movedTiles;
    for(Position item: tiles)
    {
        Position newPos = Position(item.row + rowOffset, item.col + colOffset);
        movedTiles.push_back(newPos);
    }
    return movedTiles;
}

void Block::Rotate()
{
    rotationState++;
    if(rotationState == (int)cells.size()){
        rotationState = 0;
    }
}

void Block::undoRotation()
{
    rotationState--;
    if(rotationState == -1){
        rotationState = cells.size() - 1;
    }
}
