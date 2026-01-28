#include "player.h"
#include <cstdlib>
#include "enemy.h"

Player::Player() {
    spriteSheet = LoadTexture("../assets/player_sheet_02.png");
    SetTextureFilter(spriteSheet, TEXTURE_FILTER_POINT);

    if (spriteSheet.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load player png");
    }

    position = { 32, 64 };
    oldPosition = position;
    numFrames = 3;
    currentFrame = 0;
    frameTimer = 0.0f;
    frameSpeed = 0.2f;
    frameWidth = (float)spriteSheet.width / numFrames;
    isMoving = false;
    velocity = 0.0f;
    isGrounded = false;

    playerSize = {frameWidth, (float)spriteSheet.height};
    showDebug = false;
    jumpRequest = false;
    moveDirection = IDLE;
    checkUpHeight = 10;
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
    oldPosition = position;
    int speed = 2;
    isMoving = false;

    auto isValid = [&](int x, int y) {
        if (x < 0 || x >= map.size()) return false;
        if (y < 0 || y >= map[0].size()) return false;
        return true;
    };

    if (moveDirection != IDLE) {
        isMoving = true;
        int newYHead = position.y;
        int newYFoot = position.y + playerSize.y-1;
        int newX;

        if (moveDirection == RIGHT) {
            newX = position.x + moveDirection*speed + playerSize.x-1;
        }
        else if (moveDirection == LEFT) {
            newX = position.x + moveDirection*speed;
        }

        bool isFreeHead = isValid(newX, newYHead) && !map[newX][newYHead].isSolid;
        bool isFreeFoot = isValid(newX, newYFoot) && !map[newX][newYFoot].isSolid;

        if (isFreeHead && isFreeFoot) position.x += moveDirection * speed;
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

                    //Checking if there is a block above the player
                    if (!isFreeLeft || !isFreeRight) break;
                    
                    position.y -= i;
                    position.x += moveDirection * speed;
                    break;
                    
                }
            }
        }
    }

    if (jumpRequest) {
        velocity = -6;
        isGrounded = false;
        jumpRequest = false;
    }

    if (velocity > 0) {
        velocity += 0.25f; //falling
    } 
    else {
        velocity += 0.15f; //going up
    }

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

void Player::handleInput() {
    if (IsKeyPressed(KEY_SPACE) && isGrounded) {
        jumpRequest = true;
    }

    moveDirection = 0;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) moveDirection = LEFT;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveDirection = RIGHT;
}