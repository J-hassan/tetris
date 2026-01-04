#include "game.h"
#include<random>
#include<vector>

Game::Game()
{
    grid = Grid();
    blocks = getAllBlocks();
    currentBlock = getrandomBlock();
    nextBlock = getrandomBlock();
}

Block Game::getrandomBlock()
{
    if(blocks.empty()){
        blocks = getAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::getAllBlocks()
{
    return {IBlock(),LBlock(),JBlock(),TBlock(),OBlock(),SBlock(),ZBlock()};
}

void Game::Draw()
{
    grid.Draw();
    currentBlock.Draw();
}

void Game::handleInput()
{
    int keyPressed = GetKeyPressed();
    switch(keyPressed)
    {
        case KEY_LEFT:
        moveBlockLeft();
        break;
        case KEY_RIGHT:
        moveBlockRight();
        break;
        case KEY_DOWN:
        moveBlockDown();
        break;
    }
}

void Game::moveBlockLeft()
{
    currentBlock.Move(0,-1);
    if(isBlockOutside()){
        currentBlock.Move(0,1);
    }
}

void Game::moveBlockRight()
{
    currentBlock.Move(0,1);
    if(isBlockOutside()){
        currentBlock.Move(0,-1);
    }
}

void Game::moveBlockDown()
{
    currentBlock.Move(1,0);
    if(isBlockOutside()){
        currentBlock.Move(-1,0);
    }
}

bool Game::isBlockOutside()
{
    std::vector<Position> tiles = currentBlock.getCellPositions();
    for(Position item: tiles){
        if(grid.isCellOutside(item.row,item.col)){
            return true;
        }
    }
    return false;
}
