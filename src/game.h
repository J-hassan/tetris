#pragma once
#include "grid.h"
#include "blocks.cpp"

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
    void Reset();
    void HardDrop();
    void DrawGhostPiece();
    bool TryWallKicks(int oldRotation, int newRotation);

    void UpdateBombTimer();

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

    Sound bombSound;
    Sound explosionSound;
    float bombTimer;
    float bombInterval;
    void DropBomb();

    void HoldBack();
};