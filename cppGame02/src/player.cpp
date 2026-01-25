#include "player.h"

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
}

void Player::movement() {
    int speed = 2;
    isMoving = false;
    if (IsKeyDown(KEY_A)) {playerPos.x -= 2; isMoving = true;}
    if (IsKeyDown(KEY_D)) {playerPos.x += 2; isMoving = true;}
    if (IsKeyDown(KEY_W)) {playerPos.y -= 2; isMoving = true;}
    if (IsKeyDown(KEY_S)) {playerPos.y += 2; isMoving = true;}
}

Vector2 Player::getPlayerPos() {
    return playerPos;
}

int Player::getFrameWidth() {
    return frameWidth;
}