#include "randomizer.h"
#include <algorithm>
#include <random>
#include <chrono>
#include "blocks.cpp"
  
Randomizer::Randomizer()
{
    currentIndex = 0;
    RefillBag();
}

void Randomizer::RefillBag()
{
    bag.clear();
    for (int i=1;i<=7;i++)  
    {
     bag.push_back(i);
    }
    ShuffleBag();
    currentIndex = 0;
}
void Randomizer::ShuffleBag() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);
    std::shuffle(bag.begin(), bag.end(), engine);
}
Block Randomizer::GetNextBlock()
{
    if (currentIndex >= (int)bag.size())
    {
        RefillBag();
    }
    int blockId = bag[currentIndex++];        
    switch(blockId)
    {
        case 1: return LBlock();
        case 2: return JBlock();
        case 3: return IBlock();
        case 4: return OBlock();
        case 5: return SBlock();
        case 6: return TBlock();
        case 7: return ZBlock();
        default: return IBlock(); 
    }
}

void Randomizer::Reset()
{
    bag.clear();
    currentIndex = 0;
    RefillBag();
}