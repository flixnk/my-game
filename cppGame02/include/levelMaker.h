#ifndef LEVELMAKER_H
#define LEVELMAKER_H

#include "block.h"
#include <raylib.h>
#include <vector>

class LevelMaker {
    public:
    LevelMaker();
    ~LevelMaker();

    void addPlatform(int posX, int posY, int width, int height, Block ressource, std::vector<std::vector<Block>>& levelMap, Vector2 spriteSize);
    void addEnemy();
};

#endif