#include "player.h"
#include <iostream>

Player::Player() {
  spriteSheet = LoadTexture("../assets/player_sheet_02.png");
  SetTextureFilter(spriteSheet, TEXTURE_FILTER_POINT);

  if (spriteSheet.id == 0) {
    TraceLog(LOG_ERROR, "Failed to load player png");
  }

  numFrames = 3;
  currentFrame = 0;
  frameTimer = 0.0f;
  frameSpeed = 0.2f;
  frameWidth = (float)spriteSheet.width / numFrames;

  isMoving = false;
  isGrounded = false;
  showDebug = false;
  vertVelocity = 0;
  playerPos = { 32, 8*32 };
  blockSize = 32;

  sourceRec = {0.0f, 0.0f, frameWidth, (float)spriteSheet.height};
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

void Player::movement(const std::vector<std::vector<Block>> &map) {
  int playerSize = 32;
  isMoving = false;
  float speed = 2.0f;

  if (IsKeyPressed(KEY_R)) {
    playerPos = { (float)blockSize, (float)map[0].size()*blockSize-2*blockSize};
  }

  auto isValid = [&](int x, int y) {
    if (x < 0 || x >= map.size()) return false;
    if (y < 0 || y >= map[0].size()) return false;
    return true;
  };

  if (IsKeyDown(KEY_A)) {
    isMoving = true;

    int checkX = (int)((playerPos.x - speed+1) / blockSize);
    
    int headY = (int)(playerPos.y / blockSize);
    int footY = (int)((playerPos.y + playerSize - 1) / blockSize);

    bool headFree = isValid(checkX, headY) && !map[checkX][headY].isSolid;
    bool footFree = isValid(checkX, footY) && !map[checkX][footY].isSolid;

    if (headFree && footFree) {
      playerPos.x -= speed;
    }
  };

  if (IsKeyDown(KEY_D)) {
    isMoving = true;

    int checkX = (int)((playerPos.x + playerSize - 1 + speed) / blockSize);
    
    int headY = (int)(playerPos.y / blockSize);
    int footY = (int)((playerPos.y + playerSize - 1) / blockSize);
    
    bool headFree = isValid(checkX, headY) && !map[checkX][headY].isSolid;
    bool footFree = isValid(checkX, footY) && !map[checkX][footY].isSolid;

    if (headFree && footFree) {
      playerPos.x += speed;
    }
  }
  
  if (IsKeyPressed(KEY_SPACE) && isGrounded) {
    vertVelocity = -6;
    isGrounded = false;
  }

  if (vertVelocity > 0) {
      vertVelocity += 0.35f; //falling
  } else {
      vertVelocity += 0.20f; //going up
  }

  if (vertVelocity > 20) {
    vertVelocity = 20;
}

  if (vertVelocity > 0) {
    //falling logic
    int newY = (playerPos.y + playerSize + vertVelocity)/ blockSize;

    if ((isValid(playerPos.x / blockSize, newY) && map[playerPos.x / blockSize][newY].isSolid) ||
        (isValid((playerPos.x + playerSize - 1) / blockSize, newY) && map[(playerPos.x + playerSize - 1) / blockSize][newY].isSolid)) {
      playerPos.y = (newY * blockSize) - playerSize;
      vertVelocity = 0;
      isGrounded = true;
    } else {
      playerPos.y += vertVelocity;
      isGrounded = false;
    }
  }
  else if (vertVelocity < 0) {
    //logic for going up
    int newY = (playerPos.y + vertVelocity)/ blockSize;

    if ((isValid(playerPos.x / blockSize, newY) && map[playerPos.x / blockSize][newY].isSolid) ||
        (isValid((playerPos.x + playerSize - 1) / blockSize, newY) && map[(playerPos.x + playerSize - 1) / blockSize][newY].isSolid)) {
      playerPos.y = ((newY + 1) * blockSize);
      vertVelocity = 0;
    } else {
      playerPos.y += vertVelocity;
      isGrounded = false;
    }
  }
}

Vector2 Player::getPlayerPos() {
  return playerPos;
}

void Player::reset() {
    playerPos = { 32, 8*32 };
    vertVelocity = 0;
    isGrounded = false;
    isMoving = false;
    currentFrame = 0;
}

GameScreen Player::collisionDetection(const std::vector<std::vector<Block>> &map) {
  auto isValid = [&](int x, int y) {
    if (x < 0 || x >= map.size()) return false;
    if (y < 0 || y >= map[0].size()) return false;
    return true;
  };

  int gridXLeftHead = playerPos.x/blockSize;
  int gridYLeftHead = playerPos.y/blockSize;

  bool winLeftHead = isValid(gridXLeftHead, gridYLeftHead) && map[gridXLeftHead][gridYLeftHead].isWin;
  bool lossLeftHead = isValid(gridXLeftHead, gridYLeftHead) && map[gridXLeftHead][gridYLeftHead].isDeadly;

  int gridXRightHead = (playerPos.x + blockSize - 1)/blockSize;
  int gridYRightHead = playerPos.y/blockSize;

  bool winRightHead = isValid(gridXRightHead, gridYRightHead) && map[gridXRightHead][gridYRightHead].isWin;
  bool lossRightHead = isValid(gridXRightHead, gridYRightHead) && map[gridXRightHead][gridYRightHead].isDeadly;

  int gridXLeftFoot = playerPos.x/blockSize;
  int gridYLeftFoot = (playerPos.y + blockSize - 1)/blockSize;

  bool winLeftFoot = isValid(gridXLeftFoot, gridYLeftFoot) && map[gridXLeftFoot][gridYLeftFoot].isWin;
  bool lossLeftFoot = isValid(gridXLeftFoot, gridYLeftFoot) && map[gridXLeftFoot][gridYLeftFoot].isDeadly;

  int gridXRightFoot = (playerPos.x + blockSize - 1)/blockSize;
  int gridYRightFoot = (playerPos.y + blockSize - 1)/blockSize;

  bool winRightFoot = isValid(gridXRightFoot, gridYRightFoot) && map[gridXRightFoot][gridYRightFoot].isWin;
  bool lossRightFoot = isValid(gridXRightFoot, gridYRightFoot) && map[gridXRightFoot][gridYRightFoot].isDeadly;

  if (winLeftHead || winLeftFoot || winRightFoot || winRightHead) {
    return WIN;
  } 
  else if (lossLeftHead || lossRightFoot || lossLeftFoot || lossRightHead) {
    return LOSS;
  }
  return GAMEPLAY;
}