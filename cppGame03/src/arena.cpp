#include "arena.h"
#include "raylib.h"

Tower::Tower(Vector2 position, Vector2 dimensions, Color color, TowerType towerType) {
    pos = position;
    team = color;
    type = towerType;
    size = dimensions;
}

Tower::~Tower() {}

void Tower::draw() {
    Rectangle tower = { pos.x-size.x/2, pos.y-size.y/2, size.x, size.y };
    DrawRectangleRec(tower, team);
}

Arena::Arena() {
    arena = LoadTexture("../assets/arena.png");
}

Arena::~Arena() {
    UnloadTexture(arena);
}

void Arena::draw() {
    Rectangle source = { 0, 0, float(arena.width), float(arena.height) };
    Rectangle dest = { 0, 0, float(GetScreenWidth()), float(GetScreenHeight()) };
    DrawTexturePro(arena, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);

    Tower kingTowerBlue({ float(GetScreenWidth())/9, float(GetScreenHeight())/2 }, { float(GetScreenWidth())/18, float(GetScreenWidth())/18 }, DARKBLUE, KING);
    kingTowerBlue.draw();
    Tower kingTowerRed({ float(GetScreenWidth())/9*8, float(GetScreenHeight())/2 }, { float(GetScreenWidth())/18, float(GetScreenWidth())/18 }, RED, KING);
    kingTowerRed.draw();

    Tower princessTowerBlueTop({ float(GetScreenWidth())/9*2,float(GetScreenHeight())/6.4f }, { float(GetScreenWidth())/30, float(GetScreenWidth())/30 }, DARKBLUE, PRINCESS);
    princessTowerBlueTop.draw();
    Tower princessTowerBlueBottom({ float(GetScreenWidth())/9*2,float(GetScreenHeight())/6.4f*5.4f }, { float(GetScreenWidth())/30, float(GetScreenWidth())/30 }, DARKBLUE, PRINCESS);
    princessTowerBlueBottom.draw();
    Tower princessTowerRedTop({ float(GetScreenWidth())/9*7,float(GetScreenHeight())/6.4f }, { float(GetScreenWidth())/30, float(GetScreenWidth())/30 }, RED, PRINCESS);
    princessTowerRedTop.draw();
    Tower princessTowerRedBottom({ float(GetScreenWidth())/9*7,float(GetScreenHeight())/6.4f*5.4f }, { float(GetScreenWidth())/30, float(GetScreenWidth())/30 }, RED, PRINCESS);
    princessTowerRedBottom.draw();
}