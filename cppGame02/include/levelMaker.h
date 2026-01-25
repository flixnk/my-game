#ifndef LEVELMAKER_H
#define LEVELMAKER_H

#include "block.h"
#include <raylib.h>

class levelMaker {
    public:
    levelMaker();
    ~levelMaker();

    void addPlatform(int posX, int posY, int width, int height, Block ressource, Texture2D sprite, int blockSize, int drawnBlockSize);
};

#endif