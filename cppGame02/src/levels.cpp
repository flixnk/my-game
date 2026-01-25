#include "levels.h"
#include "levelMaker.h"

Level1::Level1() {
    levelSize = { 800, 288 }; //should both be divisible by the size of your background png
    backgroundSprite = LoadTexture("../assets/light_stone_block.png");
    SetTextureFilter(backgroundSprite, TEXTURE_FILTER_POINT);
    if (backgroundSprite.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load background png.");
    }

    map.resize(levelSize.x, std::vector<Block>(levelSize.y, AIR));

    stoneBlockSprite = LoadTexture("../assets/stone_block.png");
    SetTextureFilter(backgroundSprite, TEXTURE_FILTER_POINT);
    if (backgroundSprite.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load stone_block png.");
    }
}

Level1::~Level1() {
    UnloadTexture(backgroundSprite);
    UnloadTexture(stoneBlockSprite);
}

void Level1::initLevel() {
    LevelMaker levelMaker;

    levelMaker.addPlatform(0, 0, map.size(), map[0].size(), AIR, map, backgroundSprite.height);
    levelMaker.addPlatform(0, map[0].size()-32, map.size(), 32, STONE, map, stoneBlockSprite.height);

    levelMaker.addPlatform(4, map[0].size()-60, 32, 32, STONE, map, stoneBlockSprite.height);

}

void Level1::draw() {
    for (int i = 0; i < map.size(); i+=backgroundSprite.width) {
        for (int j = 0; j < map[0].size(); j+=backgroundSprite.height) {
            DrawTexture(backgroundSprite, i, j, WHITE);
        }
    }

    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            if (map[i][j].type == STONE && map[i][j].isRenderOrigin) {
                Rectangle source = { 0, 0, (float)stoneBlockSprite.width, (float)stoneBlockSprite.height };
                Rectangle dest = { (float)i, (float)j, (float)stoneBlockSprite.width, (float)stoneBlockSprite.height };
                DrawTexturePro(stoneBlockSprite, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
            }
        }
    }
}