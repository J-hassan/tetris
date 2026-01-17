#include "game.h"
#include <random>
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

    scoreSaved = false;
    level = 1; 
    gameTimer = 0.0f;
    difficultyLevel = 1;

    canHold = true;
    hasHeldBlock = false;
    holdSound = LoadSound("Sounds/hold.mp3");

    heldBack = IBlock();
    
    // NEW: Initialize effects
    effects = new Effects();
    combo = 0;
    lastClearedLines = 0;
}

Game::~Game()
{
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadSound(hardDropSound);
    UnloadSound(holdSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
    
    // NEW: Cleanup effects
    delete effects;
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
    scoreSaved = false;
    level = 1; 
    gameTimer = 0.0f;
    difficultyLevel = 1;
    
    // NEW: Reset effects
    effects->Reset();
    combo = 0;
    lastClearedLines = 0;
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        if (difficultyLevel == 1)
        {
            blocks = {IBlock(), JBlock(), LBlock(), OBlock()};
        }
        else 
        {
            blocks = GetAllBlocks(); 
        }
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, blocks.size() - 1);
    
    int randomIndex = dist(gen); 
    Block block = blocks[randomIndex];
    
    blocks.erase(blocks.begin() + randomIndex);
    
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
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
    
    // NEW: Add landing effect
    std::vector<Color> colors = GetCellColors();
    effects->StartLandingEffect(tiles, colors[currentBlock.id]);
    
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
    
    // NEW: Get rows before clearing for animation
    std::vector<int> fullRows;
    for (int row = 0; row < 20; row++) {
        bool isFull = true;
        for (int col = 0; col < 10; col++) {
            if (grid.grid[row][col] == 0) {
                isFull = false;
                break;
            }
        }
        if (isFull) fullRows.push_back(row);
    }
    
    // NEW: Start line clear animation if rows found
    if (!fullRows.empty()) {
        effects->StartLineClearAnimation(fullRows);
        
        // Trigger screen shake for Tetris (4 lines)
        if (fullRows.size() == 4) {
            effects->StartScreenShake(8.0f, 0.3f);
        } else if (fullRows.size() >= 2) {
            effects->StartScreenShake(4.0f, 0.2f);
        }
    }
    
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        PlaySound(clearSound);
        
        // NEW: Combo system
        combo++;
        if (combo > 1) {
            effects->StartComboDisplay(combo, {250, 300});
        }
        
        UpdateScore(rowsCleared, 0);
    } else {
        // NEW: Reset combo if no lines cleared
        combo = 0;
    }
    
    canHold = true;
}

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

void Game::UpdateScore(int linesCleared, int moveDownPoints) {
    int points = 0;
    
    switch (linesCleared) {
        case 1: points = 10; break;
        case 2: points = 30; break;
        case 3: points = 60; break;
        case 4: points = 100; break;
        default: points = 0; break;
    }

    score += (points * difficultyLevel);
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