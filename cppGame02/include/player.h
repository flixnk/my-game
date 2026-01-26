#ifndef PLAYER_H
#define PLAYER_H

#include "block.h"
#include "raylib.h"
#include <vector>

class Player {
    public:
    Player();
    ~Player();

    void animate();
    void movement(const std::vector<std::vector<Block>>& map);
    void handleInput();

    Vector2 getPlayerPos();
    int getFrameWidth();
    Vector2 getPos() const { return position; }
    Vector2 getOldPos() const { return oldPosition; }

    private:
    Texture2D spriteSheet;
    Vector2 playerPos;
    int numFrames;
    int currentFrame;
    float frameTimer;
    float frameSpeed;
    float frameWidth;
    bool isMoving;
    float velocity;
    bool isGrounded;
    int playerSize;
    bool showDebug;
    bool jumpRequest;
    int moveDirection;
    Vector2 position;
    Vector2 oldPosition;
};

#endif