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
    Vector2 getPos() const { return position; }
    Vector2 getOldPos() const { return oldPosition; }
    Rectangle getHitbox() const { return { position.x, position.y, playerSize.x, playerSize.y }; }

    private:
    Texture2D spriteSheet;
    Vector2 position;
    int numFrames;
    int currentFrame;
    float frameTimer;
    float frameSpeed;
    float frameWidth;
    bool isMoving;
    float velocity;
    bool isGrounded;
    Vector2 playerSize;
    bool showDebug;
    bool jumpRequest;
    int moveDirection;
    Vector2 oldPosition;
    int checkUpHeight;
};

#endif