#include "level.h"

#include "raylib.h"
#include "block.h"
#include <iostream>

Level0::Level0() {
    levelSize = { 20, 10 };

    backgroundSprite = LoadTexture("../assets/stone_block.png");
    SetTextureFilter(backgroundSprite, TEXTURE_FILTER_POINT);

    lightStoneSprite = LoadTexture("../assets/light_stone_block.png");
    SetTextureFilter(backgroundSprite, TEXTURE_FILTER_POINT);

    flagSprite = LoadTexture("../assets/flag.png");
    SetTextureFilter(backgroundSprite, TEXTURE_FILTER_POINT);

    if (backgroundSprite.id == 0) {
      TraceLog(LOG_ERROR, "Failed to load background png");
    }

    if (lightStoneSprite.id == 0) {
      TraceLog(LOG_ERROR, "Failed to load light stone png");
    }

    if (flagSprite.id == 0) {
      TraceLog(LOG_ERROR, "Failed to load flag png");
    }

    blockSize = 32;

    map.assign(levelSize.x, std::vector<Block>(levelSize.y, Block()));
}

Level0::~Level0() { 
    UnloadTexture(backgroundSprite);
    UnloadTexture(lightStoneSprite);
    UnloadTexture(flagSprite);
}

void Level0::draw() {
  for (int x = 0; x < levelSize.x; x++) {
    map[x][levelSize.y-1] = Block(STONE, true);
  }

  map[7][levelSize.y - 2] = Block(STONE, true);

  map[8][levelSize.y - 3] = Block(STONE, true);

  map[9][levelSize.y - 4] = Block(STONE, true);

  map[levelSize.x-1][levelSize.y-2] = Block(FLAG, false, false, true);

  for (int x = 0; x < map.size(); x++) {
    for (int y = 0; y < map[0].size(); y++) {
        DrawTextureRec(lightStoneSprite, { 0, 0, 32, 32 }, { (float)x * 32, (float)y * 32 }, WHITE);
    }
  }

  Color backgroundColor = Fade(BLACK, 0.6f);
  for (int x = 0; x < map.size(); x++) {
    for (int y = 0; y < map[0].size(); y++) {

      if (map[x][y].type == AIR) {
        DrawRectangle(x*blockSize, y*blockSize, blockSize, blockSize, backgroundColor);
      }
      else if (map[x][y].type == FLAG) {
        DrawRectangle(x*blockSize, y*blockSize, blockSize, blockSize, backgroundColor);
        Rectangle source = { 0, 0, 32, 32 };
        Rectangle dest = { (float)x*32+6, (float)y*32+8, 24, 24 };
        DrawTexturePro(flagSprite, source, dest, { 0, 0 }, 0.0f, WHITE);
      }
    }
  }
}

void Level0::drawBorder() {
    Color borderColor = Fade(GREEN, 0.5f);
    for (int i = 0; i < levelSize.x; i++) {
        DrawRectangle(i*blockSize, 0, 32, 32, borderColor);
        DrawRectangle(i*blockSize, (levelSize.y-1)*32, 32, 32, borderColor);
    }
    for (int i = 0; i < levelSize.y; i++) {
        DrawRectangle(0, i*blockSize, 32, 32, borderColor);
        DrawRectangle((levelSize.x-1)*32, i*blockSize, 32, 32, borderColor);
    }
}

std::vector<std::vector<Block>> &Level0::getMap() { return map; }