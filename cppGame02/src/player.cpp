#include "player.h"
#include <cstdlib>

Player::Player() {

    spriteSheet = LoadTexture("../assets/player_sheet_02.png");
    SetTextureFilter(spriteSheet, TEXTURE_FILTER_POINT);

    if (spriteSheet.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load player png");
    }

    playerPos = { 0, 0 };
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
    moveDirection = 0;
}

Player::~Player() { UnloadTexture(spriteSheet); }

void Player::animate() {
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

    Rectangle sourceRec = {0.0f, 0.0f, frameWidth, (float)spriteSheet.height};
    sourceRec.x = (float)currentFrame * frameWidth;

    Rectangle destRec = {playerPos.x, playerPos.y, frameWidth, (float)spriteSheet.height};
    DrawTexturePro(spriteSheet, sourceRec, destRec, {0, 0}, 0.0f, WHITE);

    if (IsKeyPressed(KEY_F3)) {
        showDebug = !showDebug;
    }

    if (showDebug) {
        DrawRectangleLinesEx(destRec, 1.0f, GREEN);
    }
}

void Player::movement(const std::vector<std::vector<Block>>& map) {
    int speed = 2;
    isMoving = false;

    auto isValid = [&](int x, int y) {
        if (x < 0 || x >= map.size()) return false;
        if (y < 0 || y >= map[0].size()) return false;
        return true;
    };

    if (moveDirection != 0) {
        isMoving = true;
        playerPos.x += moveDirection * speed;
    }

    if (jumpRequest && isGrounded) {
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
    if (IsKeyPressed(KEY_SPACE)) {
        jumpRequest = true;
    }

    moveDirection = 0;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) moveDirection = -1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveDirection = 1;
    
}

Vector2 Player::getPlayerPos() {
    return playerPos;
}

int Player::getFrameWidth() {
    return frameWidth;
}