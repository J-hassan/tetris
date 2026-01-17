#pragma once
#include <raylib.h>
#include <vector>

extern const Color darkGrey;
extern const Color green;
extern const Color red;
extern const Color orange;
extern const Color yellow;
extern const Color purple;
extern const Color cyan;
extern const Color blue;
extern const Color lightBlue;
extern const Color darkBlue;
extern const Color Silver;

// NEW: Added these colors for effects
extern const Color PARTICLE_YELLOW;
extern const Color PARTICLE_ORANGE;
extern const Color PARTICLE_WHITE;

std::vector<Color> GetCellColors();