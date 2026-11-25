#include "raylib.h"
#include "raymath.h"
#include "arena.h"


int main() {
  Vector2 screenSize = {600, 400};

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenSize.x, screenSize.y, "Clash Royale Clone");
  MaximizeWindow();
  SetTargetFPS(240);
  ChangeDirectory(GetApplicationDirectory());

  Arena arena;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);

    arena.draw();

    EndDrawing();
  }
  CloseWindow();
  return 0;
}