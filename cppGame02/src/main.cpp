#include "gameCamera.h"
#include "levels.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include "userInterface.h"
#include <cmath>

enum ScreenType { MENU = 0, WIN = 1, LOSS = 2, GAME = 3 };

int main() {
  Vector2 screenSize = {600, 400};
  ScreenType currentScreen = MENU;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenSize.x, screenSize.y, "My Game02");
  MaximizeWindow();
  SetTargetFPS(144);
  ChangeDirectory(GetApplicationDirectory());

  OptionsScreen optionScreen;
  WinScreen winScreen;
  LossScreen lossScreen;
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

    if (player.getHp() == 0) {
      currentScreen = LOSS;
    } else if (level1.checkWin(player.getHitbox())) {
      currentScreen = WIN;
    }
    switch (currentScreen) {
    case MENU:
      optionScreen.draw();
      if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = GAME;
      }
      break;
    case WIN:
      winScreen.draw();
      if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU;
      }
      break;
    case LOSS:
      lossScreen.draw();
      if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU;
      }
      break;
    case GAME:
      BeginMode2D(gameCam.getRaylibCam());
      level1.draw(alpha, gameCam.getRaylibCam());
      player.animate(renderPos);
      EndMode2D();
      userInterface.draw(player.getHp());
      break;
    }

    DrawFPS(GetScreenWidth() - 100, 10);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
