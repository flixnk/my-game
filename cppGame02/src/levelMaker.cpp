#include "levelMaker.h"

LevelMaker::LevelMaker() {}

LevelMaker::~LevelMaker() {}

void LevelMaker::addPlatform(int posX, int posY, int width, int height, Block ressource, std::vector<std::vector<Block>>& levelMap, Vector2 spriteSize) {
    while (width % (int)spriteSize.x != 0) {
        width += 1;
    }

    while (height % (int)spriteSize.y != 0) {
        height += 1;
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (posX+i >= 0 && posY+j >= 0 && posX+i < levelMap.size() && posY+j < levelMap[0].size()) {
                //Works to not overwrite already existing render origins
                bool keepOldOrigin = false;
                
                if (levelMap[posX+i][posY+j].type == ressource.type) {
                     keepOldOrigin = levelMap[posX+i][posY+j].isRenderOrigin;
                }

                levelMap[posX+i][posY+j] = ressource;
                
                levelMap[posX+i][posY+j].isRenderOrigin = keepOldOrigin;
                
                if (i % (int)spriteSize.x == 0 && j % (int)spriteSize.y == 0) {
                    levelMap[posX+i][posY+j].isRenderOrigin = true;
                }
            }
        }
    }
}

void LevelMaker::addEnemy() {
    
}