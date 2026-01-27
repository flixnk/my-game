#include "levelMaker.h"

LevelMaker::LevelMaker() {}

LevelMaker::~LevelMaker() {}

void LevelMaker::addPlatform(int posX, int posY, int width, int height, Block ressource, std::vector<std::vector<Block>>& levelMap, int spriteSize) {
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
                
                if (i % spriteSize == 0 && j % spriteSize == 0) {
                    levelMap[posX+i][posY+j].isRenderOrigin = true;
                }
            }
        }
    }
}

void LevelMaker::addEnemy() {
    
}