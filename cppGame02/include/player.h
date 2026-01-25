#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

class Player {
    public:
    Player();
    ~Player();

    void animate();
    void movement();

    Vector2 getPlayerPos();
    int getFrameWidth();

    private:
    Texture2D spriteSheet;
    Vector2 playerPos;
    int numFrames;
    int currentFrame;
    float frameTimer;
    float frameSpeed;
    float frameWidth;
    bool isMoving;
};

#endif