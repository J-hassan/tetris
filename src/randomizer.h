#pragma once
#include<vector>
#include "block.h"

class Randomizer
{
    public:
        Randomizer();
        Block GetNextBlock();
        void Reset();
    private:
        std::vector<int> bag;
        int currentIndex;

        void RefillBag();
        void ShuffleBag();     
};