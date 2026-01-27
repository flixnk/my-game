#ifndef LEVELS_H
#define LEVELS_H

#include "block.h"
#include <raylib.h>
#include <vector>

class Level {
    public:
        virtual ~Level() {}
        virtual void initLevel() = 0;
        virtual void draw() = 0;

        std::vector<std::vector<Block>>& getMap() { return map; }

    protected:
        Vector2 levelSize;
        Texture2D backgroundSprite;
        std::vector<std::vector<Block>> map;
};

class Level1 : public Level {
    public:
    Level1();
    ~Level1();
    void initLevel() override;
    void draw() override;

    private:
    Texture2D stoneBlockSprite;
    Texture2D flagSprite;
};

#endif