#include "gameCamera.h"
#include "levels.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "userInterface.h"
#include <cmath>

int main() {
  Vector2 screenSize = {600, 400};

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenSize.x, screenSize.y, "FlixGame");
  MaximizeWindow();
  SetTargetFPS(144);
  ChangeDirectory(GetApplicationDirectory());

  GameCamera gameCam;
  UserInterface userInterface;
  Player player;
  Level1 level1;
  level1.initLevel();

  const std::vector<std::vector<Block>> &map = level1.getMap();

  const float targetPhysicsFPS = 128.0f;
  const float dt = 1.0f / targetPhysicsFPS;
  float accumulator = 0.0f;

  gameCam.handleResize(player.getFrameWidth());

  while (!WindowShouldClose()) {
    if (IsWindowResized()) {
      gameCam.handleResize(player.getFrameWidth());
    }

    player.handleInput();

    accumulator += GetFrameTime();
    while (accumulator >= dt) {
      player.movement(map);
      level1.updateEnemies();
      if (level1.checkCollisionWithPlayer(player.getHitbox())) {

        if (!player.getGotHit()) {
          gameCam.screenShake(8.0f);
        }
        player.triggerHit();
      }
      accumulator -= dt;
    }

    float alpha = accumulator / dt;

    Vector2 playerPosNow = player.getPos();
    Vector2 playerPosOld = player.getOldPos();
    Vector2 renderPos = Vector2Lerp(playerPosOld, playerPosNow, alpha);

    gameCam.update(renderPos, GetFrameTime());

    BeginDrawing();
    ClearBackground(BLUE);
    BeginMode2D(gameCam.getRaylibCam());
    level1.draw(alpha, gameCam.getRaylibCam());
    player.animate(renderPos);
    EndMode2D();
    userInterface.draw(player.getHp());
    DrawFPS(1800, 10);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
