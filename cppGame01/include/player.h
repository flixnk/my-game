#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include"raylib.h"
#include "block.h"

class Player {
    public:
    Player();
    ~Player();

    void movement(const std::vector<std::vector<Block>> &map);
    void animate();
    void reset();

    GameScreen collisionDetection(const std::vector<std::vector<Block>> &map);

    Vector2 getPlayerPos();

    private:
    int blockSize;
    Vector2 playerPos;
    Texture2D spriteSheet;
    int numFrames;
    int currentFrame;
    float frameTimer;
    float frameSpeed;
    float frameWidth;
    bool isMoving;
    bool isGrounded;
    bool showDebug;

    float vertVelocity;

    Rectangle sourceRec;
};

#endif