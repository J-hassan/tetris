#include "game.h"
#include <random>
#include <algorithm>
#include "wallKicks.h"

Game::Game()
{
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();

    gameOver = false;
    score = 0;
    InitAudioDevice();
    music = LoadMusicStream("Sounds/music.mp3");
    PlayMusicStream(music);
    rotateSound = LoadSound("Sounds/rotate.mp3");
    clearSound = LoadSound("Sounds/clear.mp3");
    hardDropSound = LoadSound("Sounds/harddrop.mp3");

    canHold = true;
    hasHeldBlock = false;
    holdSound = LoadSound("Sounds/hold.mp3");

    heldBack = IBlock();

    bombSound = LoadSound("Sounds/bomb.mp3");
    explosionSound = LoadSound("Sounds/explosion.mp3");
    bombTimer = 0.0f;
    bombInterval = 30.0f;
}

Game::~Game()
{
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadSound(hardDropSound);
    UnloadSound(holdSound);
    UnloadSound(bombSound);      
    UnloadSound(explosionSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock(), BombBlock()};
}

void Game::Draw()
{
    grid.Draw();
    DrawGhostPiece();
    currentBlock.Draw(11, 11);
    switch (nextBlock.id)
    {
    case 3:
        nextBlock.Draw(255, 290);
        break;
    case 4:
        nextBlock.Draw(255, 280);
        break;
    default:
        nextBlock.Draw(270, 270);
        break;
    }
    if(hasHeldBlock)
    {
        DrawText("Hold", 380, 450, 25, WHITE);

        DrawRectangleRounded({320, 480, 170, 100}, 0.3, 6, lightBlue);
        
        switch(heldBack.id)
        {
            case 3:
                heldBack.Draw(345,490);
                break;
            case 4:
                heldBack.Draw(375, 500);
                break;
            default:
                heldBack.Draw(362, 495);
                break;
        }
       

    }
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();
    if (gameOver && keyPressed != 0)
    {
        gameOver = false;
        Reset();
    }
    switch (keyPressed)
    {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    case KEY_DOWN:
        MoveBlockDown();
        UpdateScore(0, 1);
        break;
    case KEY_UP:
        RotateBlock();
        break;
    case KEY_SPACE:
        HardDrop();
        break;
    case KEY_C:
        HoldBack();
        break;    
    }
}

void Game::MoveBlockLeft()
{
    if (!gameOver)
    {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, 1);
        }
    }
}

void Game::MoveBlockRight()
{
    if (!gameOver)
    {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, -1);
        }
    }
}

void Game::MoveBlockDown()
{
    if (!gameOver)
    {
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock()
{
    if (!gameOver)
    {
        int oldRotation = currentBlock.GetRotationState();
        currentBlock.Rotate();
        int newRotation = currentBlock.GetRotationState();
        if (IsBlockOutside() || BlockFits() == false)
        {
            bool kickSuccess = TryWallKicks(oldRotation, newRotation);
            if(!kickSuccess)
            {
            currentBlock.UndoRotation();
            }
        }
        else
        {
            PlaySound(rotateSound);
        }
    }
}

bool Game::TryWallKicks(int oldRotation, int newRotation)
{
    std::vector<WallKick> wallKicks = WallKicks::GetWallKicksForBlock(currentBlock.id);
    for (const auto& kick : wallKicks)
    {
        if (kick.fromState == oldRotation && kick.toState == newRotation)
        {
            for (const auto& offset : kick.tests)
            {
                currentBlock.Move(offset.row, offset.column);
                if (!IsBlockOutside() && BlockFits())
                {
                    PlaySound(rotateSound);
                    return true;
                }
                currentBlock.Move(-offset.row, -offset.column);
            }
        }
    }
    return false;
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if (BlockFits() == false)
    {
        gameOver = true;
    }
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        PlaySound(clearSound);
        UpdateScore(rowsCleared, 0);
    }
    canHold = true;
}

// bool Game::BlockFits()
// {
//     std::vector<Position> tiles = currentBlock.GetCellPositions();
//     for (Position item : tiles)
//     {
//         if (grid.IsCellEmpty(item.row, item.column) == false)
//         {
//             return false;
//         }
//     }
//     return true;
// }

// Overloaded Function
bool Game::BlockFits(Block& block) 
{
    std::vector<Position> tiles = block.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column)) {
            return false;
        }
        if (grid.IsCellEmpty(item.row, item.column) == false) {
            return false;
        }
    }
    return true;
}

bool Game::BlockFits() 
{
    return BlockFits(currentBlock); 
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
    gameOver = false;
    isPaused = false;
    canHold = true;
    hasHeldBlock = false;
}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
{
    switch (linesCleared)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    default:
        break;
    }

    score += moveDownPoints;
}

void Game::HardDrop()
{
    if (gameOver) return;

    int dropDistance = 0;

    while(BlockFits()){
        currentBlock.Move(1,0);
        dropDistance++;
    }
    if (dropDistance > 0)
    {
    currentBlock.Move(-1,0);
    }

    if(dropDistance > 0)
    {
        UpdateScore(0,dropDistance*2);
    }
    LockBlock();

    PlaySound(hardDropSound);
}

void Game::HoldBack()
{
    if(gameOver || !canHold) 
    return;

    if (!hasHeldBlock)
    {
        heldBack = currentBlock;
        heldBack.ResetForHoldDisplay();

        if(heldBack.id == 3) 
        {
            heldBack.ResetPosition();
            heldBack.Move(1,-3); 
        } 
        else if (heldBack.id == 4) 
        {
            heldBack.ResetPosition();
            heldBack.Move(0, -4); 
        } 
        else 
        {
            heldBack.ResetPosition();
            heldBack.Move(0, -3); 
        }

        currentBlock = nextBlock;
        currentBlock.ResetPosition();

        nextBlock = GetRandomBlock();
        hasHeldBlock = true;
    }
    else
    {
        Block temp = currentBlock;
        currentBlock = heldBack;
        currentBlock.ResetPosition();

        heldBack = temp;
        if (heldBack.id == 3) 
        {
            heldBack.ResetPosition();   
            heldBack.Move(1, -3);
        } 
        else if (heldBack.id == 4) 
        {
            heldBack.ResetPosition();   
            heldBack.Move(0, -4);
        } else 
        {
            heldBack.ResetPosition();
            heldBack.Move(0, -3);
        }
    }
    canHold = false;

    PlaySound(holdSound);
}

void Game::DrawGhostPiece()
{
    Block ghost = currentBlock; 
    
    while (BlockFits(ghost)) 
    {
        ghost.Move(1, 0);
    }
    ghost.Move(-1, 0); 

    ghost.Draw(11, 11, Color{255, 255, 255, 102}); 
}

void Game::UpdateBombTimer()
{
    if (gameOver) return;
    
    bombTimer += GetFrameTime();
    
    if (bombTimer >= bombInterval) {
        DropBomb();
        bombTimer = 0.0f;
    }
}

void Game::DropBomb()
{
    if (gameOver) return;
    PlaySound(bombSound);
    int col = rand() % 10;
    int row = 0;
    while (row < 20 && grid.IsCellEmpty(row, col)) 
    {
        row++;
    }
    if (row == 20) return;
    if (!grid.IsCellEmpty(row, col)) 
    {
        int bombColor = grid.grid[row][col];
        std::vector<Position> blocksToRemove;
         bool sameColorFound = false;
        for (int r = 0; r < 20; r++) {
            for (int c = 0; c < 10; c++) {
                if (grid.grid[r][c] == bombColor) {
                    blocksToRemove.push_back(Position(r, c));
                    sameColorFound = true;
                }
            }
        }
        if (!sameColorFound) 
        {
            blocksToRemove.push_back(Position(row, col));
            int count = 0;
            for (int r = (row-2 > 0 ? row-2 : 0); r <= (row+2 < 19 ? row+2 : 19) && count < 3; r++) {
    for (int c = (col-2 > 0 ? col-2 : 0); c <= (col+2 < 9 ? col+2 : 9) && count < 3; c++)  {
                    if (!grid.IsCellEmpty(r, c) && 
                        !(r == row && c == col) &&
                        rand() % 2 == 0) {
                        blocksToRemove.push_back(Position(r, c));
                        count++;
                    }
                }
            }
        }
        for (auto& pos : blocksToRemove) {
            grid.grid[pos.row][pos.column] = 0;
        }
        grid.ClearFullRows();
        if (!blocksToRemove.empty()) 
        {
            PlaySound(explosionSound);
        }

    }
}
        