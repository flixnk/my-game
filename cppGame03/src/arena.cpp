#include "arena.h"
#include "raylib.h"
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
        Vector2 virtualMousePos = { GetMousePosition().x/GetScreenWidth()*VIRTUAL_WIDTH, GetMousePosition().y/GetScreenHeight()*VIRTUAL_HEIGHT };

        if (virtualMousePos.x < VIRTUAL_WIDTH/2.0f) {
            troops.push_back(Troop(virtualMousePos, DARKBLUE, KNIGHT));
        } else {
            troops.push_back(Troop(virtualMousePos, RED, MUSKETEER));
        }
    }

    for (auto& troop : troops) {
        troop.move(getTowerRecs());
        troop.draw();
    }
    
    if (IsKeyPressed(KEY_F1)) {
        isMiddleDrawn = not isMiddleDrawn;
    }

    if (isMiddleDrawn) {
        DrawRectangle(VIRTUAL_WIDTH/2, 0, 5, VIRTUAL_HEIGHT, GREEN);
        DrawRectangle(0, VIRTUAL_HEIGHT/2, VIRTUAL_WIDTH, 5, GREEN);
    }
}

std::vector<Rectangle> Arena::getTowerRecs() const {
    std::vector<Rectangle> towerRecs;
    
    for (auto& tower : towers) {
        towerRecs.push_back(tower.hitbox);
    }

    return towerRecs;
}

Troop::Troop(Vector2 position, Color color, TroopType troopType) {
    pos = position;
    team = color;
    type = troopType;

    switch (type) {
        case KNIGHT:
        health = 600.0f;
        attackRange = 20.0f;
        sightRange = 400.0f;
        speed = 1.0f;
        troopSize = 60.0f;
        sprite = LoadTexture("../assets/knight.png");
        break;

        case MUSKETEER:
        health = 300.0f;
        attackRange = 200.0f;
        sightRange = 400.0f;
        speed = 1.0f;
        troopSize = 40.0f;
        sprite = LoadTexture("../assets/musketeer.png");
        break;
    }
}

void Troop::move(const std::vector<Rectangle>& towerRecs) {
    Rectangle troop = { pos.x-troopSize/2, pos.y-troopSize/2, troopSize, troopSize };

    bool blockedToRight = false;
    bool blockedToLeft = false;
    bool blockedAbove = false;
    bool blockedBelow = false;

    Rectangle riverMiddle = { VIRTUAL_WIDTH/2.0f-50, 260.0f, 100, VIRTUAL_HEIGHT-520 };
    Rectangle riverTop = { VIRTUAL_WIDTH/2.0f-50, 60.0f, 100, 133 };
    Rectangle riverBottom = { VIRTUAL_WIDTH/2.0f-50, VIRTUAL_HEIGHT-60.0f-133, 100, 133 };
    
    std::vector<Rectangle> obstacles = towerRecs;
    obstacles.push_back(riverMiddle);
    obstacles.push_back(riverBottom);
    obstacles.push_back(riverTop);

    for (auto& obstacle : obstacles) {
        if (CheckCollisionRecs({troop.x+speed, troop.y, troop.width, troop.height}, obstacle)) {
            blockedToRight = true;
        }
        if (CheckCollisionRecs({troop.x-speed, troop.y, troop.width, troop.height}, obstacle)) {
            blockedToLeft = true;
        }
        if (CheckCollisionRecs({troop.x, troop.y+speed, troop.width, troop.height}, obstacle)) {
            blockedBelow = true;
        }
        if (CheckCollisionRecs({troop.x, troop.y-speed, troop.width, troop.height}, obstacle)) {
            blockedAbove = true;
        }
    }

    float distanceToTopPath = abs(pos.y-TOPPATHY);
    float distanceToBottomPath = abs(pos.y-BOTTOMPATHY);

    if (pos.y > VIRTUAL_HEIGHT/2.0f) {
        if (pos.y < BOTTOMPATHY && not blockedBelow && distanceToBottomPath > speed) {
            pos.y += speed;
        } else if (not blockedAbove && distanceToBottomPath > speed) {
            pos.y -= speed;
        }
        
    } else {
        if (pos.y < TOPPATHY && not blockedBelow && distanceToTopPath > speed) {
            pos.y += speed;
        } else if (not blockedAbove && distanceToTopPath > speed) {
            pos.y -= speed;
        }
    }
    
    if (ColorIsEqual(team, DARKBLUE) && not blockedToRight) {
        pos.x += speed;
    } else if (ColorIsEqual(team, RED) && not blockedToLeft) {
        pos.x -= speed;
    }
}

void Troop::draw() {
    Rectangle source = { 0, 0, float(sprite.width), float(sprite.height) };
    Rectangle dest = { roundf(pos.x-troopSize/2), roundf(pos.y-troopSize/2), troopSize, troopSize };
    DrawTexturePro(sprite, source, dest, {0,0}, 0.0f, team);
}