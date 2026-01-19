#include "bomb.h"
#include <raylib.h>
#include "colors.h"
#include <cmath>

Bomb::Bomb() {
    row = 0;
    column = 5;
    bombColor = 0;
    fallTimer = 0.0f;
    fallInterval = 0.3f;  // Falls every 0.3 seconds
    active = false;
}

Bomb::Bomb(int color, int startColumn) {
    row = 0;
    column = startColumn;
    bombColor = color;
    fallTimer = 0.0f;
    fallInterval = 0.3f;
    active = true;
}

void Bomb::Update(float deltaTime) {
    if (!active) return;
    
    fallTimer += deltaTime;
}

bool Bomb::ShouldFall() {
    if (!active) return false;
    
    if (fallTimer >= fallInterval) {
        fallTimer = 0.0f;
        return true;
    }
    return false;
}

void Bomb::Fall() {
    row++;
}

Position Bomb::GetPosition() const {
    return Position(row, column);
}

void Bomb::Draw(int offsetX, int offsetY) {
    if (!active) return;
    
    std::vector<Color> colors = GetCellColors();
    
    // Draw bomb body (colored cell)
    DrawRectangle(column * CELL_SIZE + offsetX, 
                  row * CELL_SIZE + offsetY, 
                  CELL_SIZE - 1, 
                  CELL_SIZE - 1, 
                  colors[bombColor]);
    
    // Draw bomb indicator (black circle in center)
    int centerX = column * CELL_SIZE + offsetX + CELL_SIZE / 2;
    int centerY = row * CELL_SIZE + offsetY + CELL_SIZE / 2;
    
    DrawCircle(centerX, centerY, 8, BLACK);
    
    // Draw fuse (small line on top)
    DrawCircle(centerX, centerY - 10, 3, ORANGE);
    DrawLineEx({(float)centerX, (float)(centerY - 10)}, 
               {(float)centerX, (float)(centerY - 5)}, 2, ORANGE);
    
    // Pulsing effect
    float pulse = (sin(GetTime() * 5.0f) + 1.0f) / 2.0f;
    Color glowColor = ORANGE;
    glowColor.a = (unsigned char)(100 * pulse);
    DrawCircle(centerX, centerY, 12, glowColor);
}