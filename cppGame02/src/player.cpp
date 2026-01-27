#include "player.h"
#include <cstdlib>
#include "enemy.h"

Player::Player() {
    spriteSheet = LoadTexture("../assets/player_sheet_02.png");
    SetTextureFilter(spriteSheet, TEXTURE_FILTER_POINT);

    if (spriteSheet.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load player png");
    }

    playerPos = { 0, 0 };
    oldPosition = playerPos;
    numFrames = 3;
    currentFrame = 0;
    frameTimer = 0.0f;
    frameSpeed = 0.2f;
    frameWidth = (float)spriteSheet.width / numFrames;
    isMoving = false;
    velocity = 0.0f;
    isGrounded = false;
    playerSize = spriteSheet.height;
    showDebug = false;
    jumpRequest = false;
    moveDirection = IDLE;
}

Player::~Player() { UnloadTexture(spriteSheet); }

void Player::animate(Vector2 renderPos) {
    if (isMoving) {
        if (currentFrame == 0)
            currentFrame = 1;

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

    Rectangle source = {0.0f, 0.0f, frameWidth, (float)spriteSheet.height};
    source.x = (float)currentFrame * frameWidth;

    Rectangle dest = {renderPos.x, renderPos.y, frameWidth, (float)spriteSheet.height};
    DrawTexturePro(spriteSheet, source, dest, {0, 0}, 0.0f, WHITE);

    if (IsKeyPressed(KEY_F3)) {
        showDebug = !showDebug;
    }

    if (showDebug) {
        DrawRectangleLinesEx(dest, 1.0f, GREEN);
    }
}

void Player::movement(const std::vector<std::vector<Block>>& map) {
    oldPosition = playerPos;
    int speed = 2;
    isMoving = false;

    auto isValid = [&](int x, int y) {
        if (x < 0 || x >= map.size()) return false;
        if (y < 0 || y >= map[0].size()) return false;
        return true;
    };

    if (moveDirection != IDLE) {
        isMoving = true;
        int newYHead = playerPos.y;
        int newYFoot = playerPos.y + playerSize-1;
        int newX;

        if (moveDirection == RIGHT) {
            newX = playerPos.x + moveDirection*speed + playerSize-1;
        }
        else if (moveDirection == LEFT) {
            newX = playerPos.x + moveDirection*speed;
        }

        bool isFreeHead = isValid(newX, newYHead) && !map[newX][newYHead].isSolid;
        bool isFreeFoot = isValid(newX, newYFoot) && !map[newX][newYFoot].isSolid;

        if (isFreeHead && isFreeFoot) playerPos.x += moveDirection * speed;
    }

    if (jumpRequest) {
        velocity = -6;
        isGrounded = false;
        jumpRequest = false;
    }

    velocity += 0.125f;

    if (velocity > 40) velocity = 40;

    int steps = abs((int)velocity); 
    int direction = (velocity > 0) ? 1 : -1;

    for (int i = 0; i < steps; i++) {
        
        int nextY = playerPos.y + direction;
        bool hitSolid = false;

        if (direction > 0) {
             int feetPos = nextY + playerSize - 1;
             
             if ((isValid(playerPos.x, feetPos) && map[playerPos.x][feetPos].isSolid) ||
                 (isValid(playerPos.x + playerSize - 1, feetPos) && map[playerPos.x + playerSize - 1][feetPos].isSolid)) {
                 hitSolid = true;
             }
        } 
        else {
             if ((isValid(playerPos.x, nextY) && map[playerPos.x][nextY].isSolid) ||
                 (isValid(playerPos.x + playerSize - 1, nextY) && map[playerPos.x + playerSize - 1][nextY].isSolid)) {
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
            playerPos.y = nextY;
            isGrounded = false;
        }
    }
}

void Player::handleInput() {
    if (IsKeyPressed(KEY_SPACE) && isGrounded) {
        jumpRequest = true;
    }

    moveDirection = 0;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) moveDirection = LEFT;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveDirection = RIGHT;
}