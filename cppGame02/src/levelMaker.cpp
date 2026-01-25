#include "levelMaker.h"
#include <stdexcept>
#include <vector>
#include "levels.h"

levelMaker::levelMaker() {}

levelMaker::~levelMaker() {}

void levelMaker::addPlatform(int posX, int posY, int width, int height, Block ressource, Texture2D sprite, int blockSize, int drawnBlockSize = -1) {
    std::vector<std::vector<Block>> levelMap = getMap();

    if (height % blockSize != 0 || width % blockSize != 0) {
        throw std::runtime_error("Error: width and height have to be mutliples of blockSize in addPlatform.");
    }

    if (drawnBlockSize == -1) {
        drawnBlockSize = blockSize;
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            levelMap[posX+i][posY+j] = ressource;

            if (i % blockSize == 0 && j % blockSize == 0) {
                Rectangle source = { 0.0f, 0.0f, (float)blockSize, (float)blockSize };
                Rectangle dest = { (float)posX+i, (float)posY+j, (float)drawnBlockSize, (float)drawnBlockSize };
                DrawTexturePro(sprite, source, dest, { 0, 0 }, 0.0f, WHITE);
            }
        }
    }
}