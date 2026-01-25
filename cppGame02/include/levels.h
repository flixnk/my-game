#ifndef LEVELS_H
#define LEVELS_H

#include "block.h"
#include <raylib.h>
#include <vector>

class Level {
    public:
        virtual ~Level() {}
        virtual void draw() = 0;

        std::vector<std::vector<Block>>& getMap() { return map; }

    protected:
        Vector2 levelSize;
        Texture2D backgroundSprite;
        std::vector<std::vector<Block>> map;
};

#endif