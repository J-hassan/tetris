#pragma once
#include "position.h"
#include <vector>

class Bomb {
public:
    Bomb();
    Bomb(int color, int startColumn);
    
    void Update(float deltaTime);
    void Draw(int offsetX, int offsetY);
    bool ShouldFall();
    void Fall();
    Position GetPosition() const;
    int GetColor() const { return bombColor; }
    bool IsActive() const { return active; }
    void Deactivate() { active = false; }
    
private:
    int row;
    int column;
    int bombColor;  // Color of the bomb (1-7)
    float fallTimer;
    float fallInterval;
    bool active;
    
    static const int CELL_SIZE = 30;
};