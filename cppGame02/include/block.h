#ifndef BLOCK_H
#define BLOCK_H

enum BlockType { AIR = 0, STONE = 1, FLAG = 13 };

typedef enum GameScreen { 
    MENU, 
    GAMEPLAY, 
    WIN,
    LOSS
} GameScreen;

#pragma once
struct Block {
    BlockType type;
    bool isSolid;
    bool isDeadly;
    bool isWin;
    bool isRenderOrigin;

    Block(BlockType id = AIR, bool collision = false, bool death = false, bool win = false, bool render = false) {
        type = id;
        isSolid = collision;
        isDeadly = death;
        isWin = win;
        isRenderOrigin = render;
    }
};

#endif