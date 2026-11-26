#include "arena.h"
#include "raylib.h"
#include <vector>

Tower::Tower(Vector2 position, float dimensions, Color color, TowerType towerType) {
    pos = position;
    team = color;
    type = towerType;
    size = dimensions;
}

Tower::~Tower() {}

void Tower::draw() {
    Rectangle hitbox = getHitbox();
    DrawRectangleRec(hitbox, team);
}

Rectangle Tower::getHitbox() const {
    Rectangle hitbox = { pos.x-size/2, pos.y-size/2, size, size };
    return hitbox;
}

Arena::Arena() {
    arena = LoadTexture("../assets/arena.png");

    kingTowerBlue = Tower({ VIRTUAL_WIDTH/9, VIRTUAL_HEIGHT/2 }, VIRTUAL_WIDTH/18, DARKBLUE, KING);
    kingTowerRed = Tower({ VIRTUAL_WIDTH/9*8, VIRTUAL_HEIGHT/2 }, VIRTUAL_WIDTH/18, RED, KING);
    princessTowerBlueTop = Tower({ VIRTUAL_WIDTH/9*2,TOPPATHY }, VIRTUAL_WIDTH/30, DARKBLUE, PRINCESS);
    princessTowerBlueBottom = Tower({ VIRTUAL_WIDTH/9*2,BOTTOMPATHY }, VIRTUAL_WIDTH/30, DARKBLUE, PRINCESS);
    princessTowerRedTop = Tower({ VIRTUAL_WIDTH/9*7,TOPPATHY }, VIRTUAL_WIDTH/30, RED, PRINCESS);
    princessTowerRedBottom = Tower({ VIRTUAL_WIDTH/9*7,BOTTOMPATHY }, VIRTUAL_WIDTH/30, RED, PRINCESS);

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
        troop.move(getTowerRecs(), troops);
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
        towerRecs.push_back(tower.getHitbox());
    }

    return towerRecs;
}

std::vector<Rectangle> Arena::getTroopRecs() const {
    std::vector<Rectangle> troopRecs;
    
    for (auto& troop : troops) {
        troopRecs.push_back(troop.getHitbox());
    }

    return troopRecs;
}

Troop::Troop(Vector2 position, Color color, TroopType troopType) {
    pos = position;
    team = color;
    type = troopType;
    isEnemyInRange = false;

    switch (type) {
        case KNIGHT:
        health = 600.0f;
        attackRange = 20.0f;
        sightRange = 400.0f;
        speed = 1.0f;
        size = 50.0f;
        sprite = LoadTexture("../assets/knight.png");
        break;

        case MUSKETEER:
        health = 300.0f;
        attackRange = 200.0f;
        sightRange = 400.0f;
        speed = 1.0f;
        size = 40.0f;
        sprite = LoadTexture("../assets/musketeer.png");
        break;
    }

}

void Troop::move(const std::vector<Rectangle>& towerRecs, const std::vector<Troop>& troops) {
    Rectangle hitbox = getHitbox();

    isEnemyInRange = false;

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
        if (CheckCollisionRecs({hitbox.x+speed, hitbox.y, hitbox.width, hitbox.height}, obstacle)) {
            blockedToRight = true;
        }
        if (CheckCollisionRecs({hitbox.x-speed, hitbox.y, hitbox.width, hitbox.height}, obstacle)) {
            blockedToLeft = true;
        }
        if (CheckCollisionRecs({hitbox.x, hitbox.y+speed, hitbox.width, hitbox.height}, obstacle)) {
            blockedBelow = true;
        }
        if (CheckCollisionRecs({hitbox.x, hitbox.y-speed, hitbox.width, hitbox.height}, obstacle)) {
            blockedAbove = true;
        }
    }

    for (const auto& troop : troops) {
        if (&troop == this || ColorIsEqual(team, troop.getTeam())) {
            continue;
        }

        if (CheckCollisionCircleRec(pos, sightRange/2.0f, troop.getHitbox())) {
            isEnemyInRange = true;
        }
    }
    

    if (isEnemyInRange) {

    }else {
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
}

Rectangle Troop::getHitbox() const {
    Rectangle hitbox = { pos.x-size/2, pos.y-size/2, size, size };
    return hitbox;
}

void Troop::draw() {
    Rectangle source = { 0, 0, float(sprite.width), float(sprite.height) };
    Rectangle dest = { roundf(pos.x-size/2), roundf(pos.y-size/2), size, size };
    DrawTexturePro(sprite, source, dest, {0,0}, 0.0f, team);
}