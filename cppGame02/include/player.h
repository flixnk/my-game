#ifndef PLAYER_H
#define PLAYER_H

#include "block.h"
#include "raylib.h"
#include <vector>

class Player {
    public:
    Player();
    ~Player();

    void animate(Vector2 renderPos);
    void movement(const std::vector<std::vector<Block>>& map);
    void handleInput();

    int getFrameWidth() const { return frameWidth; }
    Vector2 getPos() const { return playerPos; }
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
    Vector2 oldPosition;
};

#endif