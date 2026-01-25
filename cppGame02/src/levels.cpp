#include "levels.h"

class Level1 : public Level {
    public:
    Level1() {
        levelSize = { 800, 600 };
        backgroundSprite = LoadTexture("../assets/light_stone_block.png");
        SetTextureFilter(backgroundSprite, TEXTURE_FILTER_POINT);
        if (backgroundSprite.id == 0) {
            TraceLog(LOG_ERROR, "Failed to load background png.");
        }
        map.resize(levelSize.x, std::vector<Block>(levelSize.y, AIR));
    }
};