#include "raylib.h"
#include "raymath.h"
#include "arena.h"


int main() {
  Vector2 screenSize = {1280, 720};

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenSize.x, screenSize.y, "Clash Royale Clone");
  MaximizeWindow();
  SetTargetFPS(240);
  ChangeDirectory(GetApplicationDirectory());

  RenderTexture2D target = LoadRenderTexture((int)VIRTUAL_WIDTH, (int)VIRTUAL_HEIGHT);

  Arena arena;

  while (!WindowShouldClose()) {
    BeginTextureMode(target);
      ClearBackground(LIGHTGRAY);
      arena.draw();
    EndTextureMode();

    BeginDrawing();
      ClearBackground(LIGHTGRAY);
      Rectangle src = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
      Rectangle dst = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
      DrawTexturePro(target.texture, src, dst, { 0.0f, 0.0f }, 0.0f, WHITE);

      DrawFPS(GetScreenWidth() - 100, 10);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}