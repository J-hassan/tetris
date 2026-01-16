#pragma once
#include <vector>
#include "position.h"

struct WallKick
{
    int fromState;
    int toState;
    std::vector<Position> tests;
};

class WallKicks
{
    public:
    static std::vector<WallKick> GetWallKicksForBlock(int blockId);
    static std::vector<WallKick> GetStandardWallKicks();
    static std::vector<WallKick> GetIBlockWallKicks();
    static std::vector<WallKick> GetOBlockWallKicks();
};