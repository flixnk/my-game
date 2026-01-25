#include "raylib.h"
#include <iostream>
#include <vector>
#include "level.h"
#include "player.h"

int main() {
    Vector2 screenSize = { 200, 150 };

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenSize.x,screenSize.y, "My Game01");
    MaximizeWindow();
    SetTargetFPS(144);
    ChangeDirectory(GetApplicationDirectory());

    Camera2D camera = { 0 };
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;

    Player player;
    Level0 level;

    GameScreen currentScreen = MENU;

    while (!WindowShouldClose()) {
        if (IsWindowResized()) {
            screenSize.x = GetScreenWidth();
            screenSize.y = GetScreenHeight();

            camera.offset.x = screenSize.x / 2;
            camera.offset.y = screenSize.y / 2;
        }

        std::vector<std::vector<Block>> levelMap = level.getMap();


        switch(currentScreen) {
            case MENU:
            {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    currentScreen = GAMEPLAY;
                }
            } break;

            case GAMEPLAY:
            {
                player.movement(levelMap);
                Vector2 playerPos = player.getPlayerPos();
                Vector2 targetTarget = { playerPos.x + 16.0f, playerPos.y - 16.0f };

                currentScreen = player.collisionDetection(levelMap);

                float smoothSpeed = 5.0f * GetFrameTime(); 
                camera.target.x += (targetTarget.x - camera.target.x) * smoothSpeed;
                camera.target.y += (targetTarget.y - camera.target.y) * smoothSpeed;
            } break;

            case WIN:
            {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    player.reset();
                    currentScreen = MENU; 
                }
            } break;

            case LOSS:
            {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                    player.reset();
                    currentScreen = MENU; 
                }
            } break;
        }
        

        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch(currentScreen) {
                case MENU:
                {
                    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
                    DrawText("START MENU", 100, 100, 40, WHITE);
                    DrawText("Drücke ENTER zum Starten", 100, 160, 20, GRAY);
                } break;

                case GAMEPLAY:
                {
                    DrawRectangle(-320, -320, 10000, 8000, BLUE);
                    BeginMode2D(camera);
                        level.draw();
                        //level.drawBorder();
                        //DrawTriangle({ 0, (float)levelMap[0].size()*32-32 }, { 32, (float)levelMap[0].size()*32-32 }, { 16,  (float)levelMap[0].size()*32-52 }, GREEN);

                        player.animate();
                    EndMode2D();
                } break;

                case WIN:
                {
                    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
                    DrawText("GEWONNEN!", 100, 100, 40, BLACK);
                    DrawText("Drücke ENTER für Neustart", 100, 160, 20, DARKGRAY);
                } break;
                case LOSS:
                {
                    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RED);
                    DrawText("VERLOREN!", 100, 100, 40, BLACK);
                    DrawText("Drücke ENTER für Neustart", 100, 160, 20, DARKGRAY);
                } break;
            }
        EndDrawing();    
    }
    CloseWindow();
    return 0;
}