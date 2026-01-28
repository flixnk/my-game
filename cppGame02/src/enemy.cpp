#include "enemy.h"
#include <cstdlib>

Enemy::Enemy(Vector2 enemyPos, Texture2D sprite) {
    coinEnemySprite = sprite;
    position = enemyPos;
    oldEnemyPos = position;
    numFrames = 3;
    currentFrame = 0;
    frameTimer = 0.0f;
    frameSpeed = 0.2f;
    frameWidth = (float)coinEnemySprite.width / numFrames;
    playerSize = { frameWidth, (float)coinEnemySprite.height };
    hitboxSize = { frameWidth/1.75f, (float)coinEnemySprite.height/1.75f };
    isMoving = false;
    moveDirection = LEFT;
    velocity = 0;
    isGrounded = false;
    showDebug = false;
    checkUpHeight = 10;
}

Enemy::~Enemy() {}

void Enemy::animate(Vector2 enemyRenderPos) {
    if (isMoving) {
        if (currentFrame == 0) currentFrame = 1;

        frameTimer += GetFrameTime();
        if (frameTimer >= frameSpeed) {
            frameTimer = 0.0f;
            currentFrame++;

        if (currentFrame >= numFrames)
            currentFrame = 1;
        }
    } else {
        currentFrame = 0;
    }

    Rectangle source = {0.0f, 0.0f, frameWidth, (float)coinEnemySprite.height};
    source.x = (float)currentFrame * frameWidth;

    if (moveDirection == LEFT) {
        source.width = -frameWidth;
    } else {
        source.width = frameWidth;
    }

    Rectangle dest = {enemyRenderPos.x, enemyRenderPos.y, frameWidth, (float)coinEnemySprite.height};
    DrawTexturePro(coinEnemySprite, source, dest, {0, 0}, 0.0f, WHITE);

    if (IsKeyPressed(KEY_F3)) {
        showDebug = !showDebug;
    }

    if (showDebug) {
        float offsetX = (frameWidth - hitboxSize.x) / 2.0f;
        float offsetY = (coinEnemySprite.height - hitboxSize.y) / 2.0f;

        Rectangle debugRect = { 
            enemyRenderPos.x + offsetX, 
            enemyRenderPos.y + offsetY, 
            hitboxSize.x, 
            hitboxSize.y 
        };

        DrawRectangleLinesEx(debugRect, 1.0f, RED);
    }
}

void Enemy::movement(const std::vector<std::vector<Block>>& map) {
    oldEnemyPos = position;
    float speed = 1.0f;
    isMoving = false;

    auto isValid = [&](int x, int y) {
        if (x < 0 || x >= map.size()) return false;
        if (y < 0 || y >= map[0].size()) return false;
        return true;
    };

    if (moveDirection != IDLE && isGrounded) {
        int newYHead = position.y;
        int newYFoot = position.y + playerSize.y-1;
        int newX = 0;

        if (moveDirection == RIGHT) {
            newX = position.x + moveDirection*speed + playerSize.x-1;
        }
        else if (moveDirection == LEFT) {
            newX = position.x + moveDirection*speed;
        }

        bool isFreeHead = isValid(newX, newYHead) && !map[newX][newYHead].isSolid;
        bool isFreeFoot = isValid(newX, newYFoot) && !map[newX][newYFoot].isSolid;

        bool hasMoved = false;

        if (isFreeHead && isFreeFoot) { 
            position.x += moveDirection * speed; 
            isMoving = true;
            hasMoved = true;
        }
        else if (isFreeHead) {
            //Logic for going up small blocks
            for (int i = 1; i <= checkUpHeight; i++) {
                int checkUpY = newYFoot - i;
                if (isValid(newX, checkUpY) && !map[newX][checkUpY].isSolid) {
                    int checkAbove = position.y - i;
                    int checkAboveLeft = position.x;
                    int checkAboveRight = position.x + playerSize.x -1;

                    bool isFreeLeft = isValid(checkAboveLeft, checkAbove) && !map[checkAboveLeft][checkAbove].isSolid;
                    bool isFreeRight = isValid(checkAboveRight, checkAbove) && !map[checkAboveRight][checkAbove].isSolid;

                    //Checking if there is a block above the enemy
                    if (!isFreeLeft || !isFreeRight) break;
                    
                    position.y -= i;
                    position.x += moveDirection * speed;
                    hasMoved = true;
                    break;
                    
                }
            }
        }

        if (!hasMoved) {
            if (moveDirection == RIGHT) moveDirection = LEFT;
            else if (moveDirection == LEFT) moveDirection = RIGHT;
        }
    }

    velocity += 0.125f;

    if (velocity > 40) velocity = 40;

    int steps = abs((int)velocity); 
    int direction = (velocity > 0) ? 1 : -1;

    for (int i = 0; i < steps; i++) {
        
        int nextY = position.y + direction;
        bool hitSolid = false;

        if (direction > 0) {
             int feetPos = nextY + playerSize.y - 1;
             
             if ((isValid(position.x, feetPos) && map[position.x][feetPos].isSolid) ||
                 (isValid(position.x + playerSize.x - 1, feetPos) && map[position.x + playerSize.x - 1][feetPos].isSolid)) {
                 hitSolid = true;
             }
        } 
        else {
             if ((isValid(position.x, nextY) && map[position.x][nextY].isSolid) ||
                 (isValid(position.x + playerSize.x - 1, nextY) && map[position.x + playerSize.x - 1][nextY].isSolid)) {
                 hitSolid = true;
             }
        }

        if (hitSolid) {
            velocity = 0;
            if (direction > 0) {
                isGrounded = true; 
            }
            break;
        } else {
            position.y = nextY;
            isGrounded = false;
        }
    }
}

Rectangle Enemy::getHitbox() const {
    float offsetX = (frameWidth - hitboxSize.x) / 2.0f;
    float offsetY = (coinEnemySprite.height - hitboxSize.y) / 2.0f;

    return { 
        position.x + offsetX, 
        position.y + offsetY, 
        hitboxSize.x, 
        hitboxSize.y 
    };
}