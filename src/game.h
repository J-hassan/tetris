#pragma once
#include "grid.h"
#include "blocks.cpp"
#include "effects.h"
#include "bomb.h"

class Game
{
public:
    Game();
    ~Game();
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    bool gameOver;
    int score;
    Music music;
    bool isPaused = false;
    bool scoreSaved;
    void Reset();
    void HardDrop();
    void DrawGhostPiece();
    bool TryWallKicks(int oldRotation, int newRotation);
    int level;
    float gameTimer;
    int difficultyLevel;
    
    Effects* GetEffects() { return effects; }
    int combo;
    
    // Bomb system (public if called externally)
    void UpdateBomb(float deltaTime);
    void SpawnBomb();
    void ExplodeBomb();

private:
    void MoveBlockLeft();
    void MoveBlockRight();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    bool BlockFits(Block& block);
    void UpdateScore(int linesCleared, int moveDownPoints);
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Sound rotateSound;
    Sound clearSound;
    Sound hardDropSound;

    Block heldBack;
    bool canHold;
    bool hasHeldBlock;
    Sound holdSound;

    void HoldBack();
    
    Effects* effects;
    int lastClearedLines;
    
    Bomb activeBomb;
    bool hasBomb;
    Sound bombSound;
    Sound explosionSound;
    
    int CalculateBlastRadius(int bombColor, int targetColor);
    void ClearCellsInRadius(int centerRow, int centerCol, int radius);
    bool ShouldSpawnBomb();
};