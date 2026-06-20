#include "gameCamera.h"
#include "levels.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include "userInterface.h"
#include "utilities.h"

int main() {
  Vector2 screenSize = {600, 400};
  ScreenType currentScreen = MENU;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenSize.x, screenSize.y, "My Game02");
  InitAudioDevice();
  MaximizeWindow();
  SetTargetFPS(144);
  ChangeDirectory(GetApplicationDirectory());

  MenuScreen menuScreen;
  WinScreen winScreen;
  LossScreen lossScreen;
  OptionsScreen optionsScreen;
  CreditsScreen creditsScreen;
  GameCamera gameCam;
  UserInterface userInterface;
  Player player;
  Level1 level1;
  level1.initLevel();

  Music music = LoadMusicStream("../assets/DDR.mp3");
  PlayMusicStream(music);

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
      if (currentScreen == GAME) {
        player.movement(map, dt);
        level1.updateEnemies();
      }
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
      currentScreen = menuScreen.update();
      menuScreen.draw();
      break;
    case WIN:      
      currentScreen = winScreen.update();
      winScreen.draw();

      if (currentScreen == MENU) {
        player.reset();
      }
      break;
    case LOSS:      
      currentScreen = lossScreen.update();
      lossScreen.draw();

      if (currentScreen == MENU) {
        player.reset();
      }
      break;
    case GAME:
      UpdateMusicStream(music);
      SetMusicVolume(music, 0.1f);
      BeginMode2D(gameCam.getRaylibCam());
      level1.draw(alpha, gameCam.getRaylibCam());
      player.animate(renderPos);
      EndMode2D();
      userInterface.draw(player.getHp());
      break;
    case OPTIONS:
      currentScreen = optionsScreen.update();
      optionsScreen.draw();
      break;
    case CREDITS:
      currentScreen = creditsScreen.update();
      creditsScreen.draw();
    }

    DrawFPS(GetScreenWidth() - 100, 10);
    EndDrawing();
  }
  UnloadMusicStream(music);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
