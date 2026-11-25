#include "arena.h"
#include "raylib.h"
#include <cstdlib>
#include <vector>

Tower::Tower(Vector2 position, Vector2 dimensions, Color color, TowerType towerType) {
    pos = position;
    team = color;
    type = towerType;
    size = dimensions;
    hitbox = { pos.x-size.x/2, pos.y-size.y/2, size.x, size.y };
}

Tower::~Tower() {}

void Tower::draw() {
    DrawRectangleRec(hitbox, team);
}

Arena::Arena() {
    arena = LoadTexture("../assets/arena.png");

    kingTowerBlue = Tower({ VIRTUAL_WIDTH/9, VIRTUAL_HEIGHT/2 }, { VIRTUAL_WIDTH/18, VIRTUAL_WIDTH/18 }, DARKBLUE, KING);
    kingTowerRed = Tower({ VIRTUAL_WIDTH/9*8, VIRTUAL_HEIGHT/2 }, { VIRTUAL_WIDTH/18, VIRTUAL_WIDTH/18 }, RED, KING);
    princessTowerBlueTop = Tower({ VIRTUAL_WIDTH/9*2,TOPPATHY }, { VIRTUAL_WIDTH/30, VIRTUAL_WIDTH/30 }, DARKBLUE, PRINCESS);
    princessTowerBlueBottom = Tower({ VIRTUAL_WIDTH/9*2,BOTTOMPATHY }, { VIRTUAL_WIDTH/30, VIRTUAL_WIDTH/30 }, DARKBLUE, PRINCESS);
    princessTowerRedTop = Tower({ VIRTUAL_WIDTH/9*7,TOPPATHY }, { VIRTUAL_WIDTH/30, VIRTUAL_WIDTH/30 }, RED, PRINCESS);
    princessTowerRedBottom = Tower({ VIRTUAL_WIDTH/9*7,BOTTOMPATHY }, { VIRTUAL_WIDTH/30, VIRTUAL_WIDTH/30 }, RED, PRINCESS);

    isMiddleDrawn = false;

    towers.push_back(kingTowerBlue);
    towers.push_back(kingTowerRed);
    towers.push_back(princessTowerBlueTop);
    towers.push_back(princessTowerBlueBottom);
    towers.push_back(princessTowerRedTop);
    towers.push_back(princessTowerRedBottom);
}

Arena::~Arena() {
    UnloadTexture(arena);
}

void Arena::draw() {
    Rectangle source = { 0, 0, float(arena.width), float(arena.height) };
    Rectangle dest = { 0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT };
    DrawTexturePro(arena, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);

    kingTowerBlue.draw();
    kingTowerRed.draw();

    princessTowerBlueTop.draw();
    princessTowerBlueBottom.draw();
    princessTowerRedTop.draw();
    princessTowerRedBottom.draw();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        troops.push_back(Troop({GetMousePosition().x/GetScreenWidth()*VIRTUAL_WIDTH, GetMousePosition().y/GetScreenHeight()*VIRTUAL_HEIGHT }, DARKBLUE));
    }

    for (auto& troop : troops) {
        troop.move(getTowerRecs());
        troop.draw();
    }

    
    if (IsKeyPressed(KEY_F1)) {
        isMiddleDrawn = not isMiddleDrawn;
    }

    if (isMiddleDrawn) {
        DrawRectangle(VIRTUAL_WIDTH/2, 0, 2, VIRTUAL_HEIGHT, GREEN);
        DrawRectangle(0, VIRTUAL_HEIGHT/2, VIRTUAL_WIDTH, 2, GREEN);
    }
}

std::vector<Rectangle> Arena::getTowerRecs() const {
    std::vector<Rectangle> towerRecs;
    
    for (auto& tower : towers) {
        towerRecs.push_back(tower.hitbox);
        towerRecs.push_back(tower.hitbox);
        towerRecs.push_back(tower.hitbox);
        towerRecs.push_back(tower.hitbox);
        towerRecs.push_back(tower.hitbox);
        towerRecs.push_back(tower.hitbox);
    }

    return towerRecs;
}

Troop::Troop(Vector2 position, Color color) {
    pos = position;
    team = color;
    speed = 1.0f;
    troopSize = 30.0f;
}

Troop::~Troop() {}

void Troop::move(const std::vector<Rectangle>& towerRecs) {
    Rectangle troop = { pos.x-troopSize/2, pos.y-troopSize/2, troopSize, troopSize };

    bool isTowerToRight = false;
    bool isTowerToLeft = false;
    bool isTowerAbove = false;
    bool isTowerBelow = false;

    for (auto& tower : towerRecs) {
        if (CheckCollisionRecs({troop.x+1, troop.y, troop.width, troop.height}, tower)) {
            isTowerToRight = true;
        }
        if (CheckCollisionRecs({troop.x-1, troop.y, troop.width, troop.height}, tower)) {
            isTowerToLeft = true;
        }
        if (CheckCollisionRecs({troop.x, troop.y+1, troop.width, troop.height}, tower)) {
            isTowerBelow = true;
        }
        if (CheckCollisionRecs({troop.x, troop.y-1, troop.width, troop.height}, tower)) {
            isTowerAbove = true;
        }
    }

    if (pos.y > VIRTUAL_HEIGHT/2.0f) {
        if (pos.y < BOTTOMPATHY && not isTowerBelow) {
            pos.y += speed;
        } else if (not isTowerAbove) {
            pos.y -= speed;
        }
        
    } else {
        if (pos.y < TOPPATHY && not isTowerBelow) {
            pos.y += speed;
        } else if (not isTowerAbove) {
            pos.y -= speed;
        }
        
    }

    if (not isTowerToRight) {
        pos.x += speed;
    }
    
}

void Troop::draw() {
    Rectangle troop = { pos.x-troopSize/2, pos.y-troopSize/2, troopSize, troopSize };
    DrawRectangleRec(troop, team);
}