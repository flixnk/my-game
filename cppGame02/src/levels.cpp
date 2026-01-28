#include "levels.h"
#include "levelMaker.h"
#include <raymath.h>

void Level::updateEnemies() {
    for (auto& enemy : enemies) {
        enemy.movement(map);
    }
}

bool Level::checkCollisionWithPlayer(Rectangle playerHitbox) {
    for (const auto& enemy : enemies) {
        if (CheckCollisionRecs(playerHitbox, enemy.getHitbox())) {
            return true;
        }
    }
    return false;
}

Level1::Level1() {
    levelSize = { 8000, 300 }; //should both be divisible by the size of your background png
    backgroundSprite = LoadTexture("../assets/light_stone_block.png");
    SetTextureFilter(backgroundSprite, TEXTURE_FILTER_POINT);
    if (backgroundSprite.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load background png.");
    }

    while ((int)levelSize.x % backgroundSprite.height != 0) {
        levelSize.x += 1;
    }

    while ((int)levelSize.y % backgroundSprite.width != 0) {
        levelSize.y += 1;
    }

    map.resize(levelSize.x, std::vector<Block>(levelSize.y, AIR));

    stoneBlockSprite = LoadTexture("../assets/stone_block.png");
    SetTextureFilter(backgroundSprite, TEXTURE_FILTER_POINT);
    if (stoneBlockSprite.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load stone_block png.");
    }

    flagSprite = LoadTexture("../assets/flag.png");
    SetTextureFilter(flagSprite, TEXTURE_FILTER_POINT);
    if (flagSprite.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load flag png.");
    }

    coinEnemySprite = LoadTexture("../assets/coin_enemy.png");
    SetTextureFilter(coinEnemySprite, TEXTURE_FILTER_POINT);
    if (coinEnemySprite.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load coin enemy png.");
    }
}

Level1::~Level1() {
    UnloadTexture(backgroundSprite);
    UnloadTexture(stoneBlockSprite);
    UnloadTexture(flagSprite);
    UnloadTexture(coinEnemySprite);
}

void Level1::initLevel() {
    LevelMaker levelMaker;

    const int STS = 32; //STANDARD_TILE_SIZE
    const int mapW = map.size();
    const int mapH = map[0].size();

    Vector2 stoneSize = { (float)stoneBlockSprite.height, (float)stoneBlockSprite.width };
    Vector2 flagSize = { (float)flagSprite.height, (float)flagSprite.width };

    levelMaker.addPlatform(0, mapH-STS, mapW, STS, { STONE, true}, map, stoneSize);

    levelMaker.addPlatform(STS*5, mapH-STS-10, STS, STS, { STONE, true}, map, stoneSize);
    levelMaker.addPlatform(STS*6, mapH-STS*3, STS, STS, { STONE, true}, map, stoneSize);
    levelMaker.addPlatform(STS*3, mapH-STS*4, STS, STS*3, { STONE, true}, map, stoneSize);

    levelMaker.addPlatform(mapW-STS, mapH-STS*2, STS, STS, { FLAG, false, false, true }, map, flagSize);

    renderList.clear(); 
    
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            if (map[i][j].isRenderOrigin) {
                renderList.push_back({ 
                    {(float)i, (float)j},
                    map[i][j].type
                });
            }
        }
    }

    enemies.push_back(Enemy({ STS*7, STS*3 }, coinEnemySprite));
    enemies.push_back(Enemy({ STS*10, STS*3 }, coinEnemySprite));
}

void Level1::draw(float alpha, Camera2D camera) {
    int startX = (int)(camera.target.x - (camera.offset.x / camera.zoom)) - 100;
    int endX   = (int)(camera.target.x + ((GetScreenWidth() - camera.offset.x) / camera.zoom)) + 100;

    if (startX < 0) startX = 0;
    if (endX > map.size()) endX = map.size();

    int bgStart = (startX / backgroundSprite.width) * backgroundSprite.width;

    for (int i = bgStart; i < endX; i+= backgroundSprite.width) {
        for (int j = 0; j < map[0].size(); j+=backgroundSprite.height) {
            DrawTexture(backgroundSprite, i, j, WHITE);
        }
    }

    for (const auto& obj : renderList) {
        if (obj.position.x < startX || obj.position.x > endX) {
            continue; 
        }

        if (obj.type == STONE) {
            DrawTexture(stoneBlockSprite, (int)obj.position.x, (int)obj.position.y, WHITE);
        }
        else if (obj.type == FLAG) {
            DrawTexture(flagSprite, (int)obj.position.x, (int)obj.position.y, WHITE);
        }
    }

    for (auto& enemy : enemies) {
        Vector2 posNow = enemy.getPos();
        Vector2 posOld = enemy.getOldPos();

        Vector2 renderPos = Vector2Lerp(posOld, posNow, alpha);

        enemy.animate(renderPos);
    }
}
