#ifndef ENEMY_H
#define ENEMY_H

#include "block.h"
#include <raylib.h>
#include <vector>

enum Direction {
    LEFT = -1,
    IDLE = 0,
    RIGHT = 1
};

class Enemy {
    public:
    Enemy(Vector2 enemyPos, Texture2D sprite);
    ~Enemy();
    void animate(Vector2 enemyRenderPos);
    void movement(const std::vector<std::vector<Block>>& map);

    Vector2 getPos() const { return position; }
    Vector2 getOldPos() const { return oldEnemyPos; }
    Rectangle getHitbox() const;

    private:
    Texture2D coinEnemySprite;
    Vector2 position;
    int numFrames;
    int currentFrame;
    float frameTimer;
    float frameSpeed;
    float frameWidth;
    bool isMoving;
    int moveDirection;
    Vector2 playerSize;
    Vector2 hitboxSize;
    float velocity;
    bool isGrounded;
    Vector2 oldEnemyPos;
    bool showDebug;
};

#endif