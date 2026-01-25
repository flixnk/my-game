#include "raylib.h"
#include "player.h"
#include <cmath>

int main() {
    Vector2 screenSize = { 200, 150 };

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenSize.x,screenSize.y, "My Game02");
    MaximizeWindow();
    SetTargetFPS(144);
    ChangeDirectory(GetApplicationDirectory());

    Camera2D camera = { 0 };
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;

    Player player;

    while (!WindowShouldClose()) {
        if (IsWindowResized()) {
            screenSize.x = GetScreenWidth();
            screenSize.y = GetScreenHeight();

            int frameWidth = player.getFrameWidth();
            camera.offset.x = floorf(screenSize.x / 2.0f - frameWidth*camera.zoom/2);
            camera.offset.y = floorf(screenSize.y / 2.0f - frameWidth*camera.zoom/2);
        }

        player.movement();
        Vector2 playerPos = player.getPlayerPos();

        camera.target.x = playerPos.x;
        camera.target.y = playerPos.y;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                player.animate();
            EndMode2D();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}