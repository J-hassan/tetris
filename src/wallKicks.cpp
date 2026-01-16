#include "wallKicks.h"
#include <vector>

std::vector<WallKick> WallKicks::GetStandardWallKicks()
{
    std::vector<WallKick> kicks;

    WallKick kick01;
    kick01.fromState = 0;
    kick01.toState = 1;
    kick01.tests = { 
        Position(0,0), 
        Position(0,-1),
        Position(0,1),
        Position(-1,0),
        Position(1,0)};
        kicks.push_back(kick01);

    WallKick kick12;
    kick12.fromState = 1;
    kick12.toState = 2;
    kick12.tests = { 
        Position(0,0), 
        Position(0,1),
        Position(0,-1),
        Position(-1,0),
        Position(1,0)};
        kicks.push_back(kick12);

    WallKick kick21;
    kick21.fromState = 2;
    kick21.toState = 1;
    kick21.tests = { 
        Position(0,0), 
        Position(0,-1),
        Position(0,1),
        Position(1,0),
        Position(-1,0)};
        kicks.push_back(kick21);

    WallKick kick23;
    kick23.fromState = 2;
    kick23.toState = 3;
    kick23.tests = { 
        Position(0,0), 
        Position(0,1),
        Position(0,-1),
        Position(1,0),
        Position(-1,0)};
        kicks.push_back(kick23);    

    WallKick kick32;
    kick32.fromState = 3;
    kick32.toState = 2;
    kick32.tests = { 
        Position(0,0), 
        Position(0,-1),
        Position(0,1),
        Position(-1,0),
        Position(1,0)};
        kicks.push_back(kick32);
    
    WallKick kick30;
    kick30.fromState = 3;
    kick30.toState = 0;
    kick30.tests = { 
        Position(0,0), 
        Position(0,-1),
        Position(0,1),
        Position(1,0),
        Position(-1,0)};
        kicks.push_back(kick30);    

    WallKick kick03;
    kick03.fromState = 0;
    kick03.toState = 3;
    kick03.tests = { 
        Position(0,0), 
        Position(0,1),
        Position(0,-1),
        Position(-1,0),
        Position(1,0)};
        kicks.push_back(kick03);

    return kicks;
}

std::vector<WallKick> WallKicks::GetIBlockWallKicks()
{
    std::vector<WallKick> kicks;

    WallKick kick01;
    kick01.fromState = 0;
    kick01.toState = 1;
    kick01.tests = { 
        Position(0,0), 
        Position(0,-2),
        Position(0,1),
        Position(-0,-1),
        Position(0,2)};
        kicks.push_back(kick01);

    return kicks;
}

std::vector<WallKick> WallKicks::GetOBlockWallKicks()
{
    return {};
}

std::vector<WallKick> WallKicks::GetWallKicksForBlock(int blockId)
{
    switch(blockId)
    {
        case 3:
            return GetIBlockWallKicks();
        case 4: 
            return GetOBlockWallKicks();
        default: 
            return GetStandardWallKicks();
    }
}