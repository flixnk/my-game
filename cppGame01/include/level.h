#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "block.h"
#include "raylib.h"

class Level0 {
    public:
        void draw();
        void drawBorder();
        Level0();
        ~Level0();
        std::vector<std::vector<Block>>& getMap();

    private:
        int blockSize;
        Vector2 levelSize;
        Texture2D backgroundSprite;
        Texture2D lightStoneSprite; 
        Texture2D flagSprite;
        std::vector<std::vector<Block>> map;
};

#endif