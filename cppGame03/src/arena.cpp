#include "arena.h"
#include "raylib.h"
#include <vector>
#include "raymath.h"

Tower::Tower(Vector2 position, Color color, TowerType towerType) {
    pos = position;
    team = color;
    type = towerType;
    health = maxHealth;

    switch (type) {
        case (KING):
        size = 100.0f;
        break;

        case (PRINCESS):
        size = 60.0f;
        break;
    }
}

Tower::~Tower() {}

void Tower::draw() {
    float healthBarWidth = 80.0f;
    Rectangle healthbarRed = { pos.x - healthBarWidth/2.0f, pos.y-50.0f, healthBarWidth, 10.0f };
    DrawRectangleRec(healthbarRed, RED);

    float healthRemainingPercent = health/maxHealth;
    Rectangle healthBarGreen = { healthbarRed.x, healthbarRed.y, healthbarRed.width*healthRemainingPercent, healthbarRed.height };
    DrawRectangleRec(healthBarGreen, GREEN);

    Rectangle hitbox = getHitbox();
    DrawRectangleRec(hitbox, team);
}

Rectangle Tower::getHitbox() const {
    Rectangle hitbox = { pos.x-size/2, pos.y-size/2, size, size };
    return hitbox;
}

Arena::Arena() {
    arena = LoadTexture("../assets/arena.png");

    kingTowerBlue = Tower({ VIRTUAL_WIDTH/9, VIRTUAL_HEIGHT/2 }, DARKBLUE, KING);
    kingTowerRed = Tower({ VIRTUAL_WIDTH/9*8, VIRTUAL_HEIGHT/2 }, RED, KING);
    princessTowerBlueTop = Tower({ VIRTUAL_WIDTH/9*2,TOPPATHY }, DARKBLUE, PRINCESS);
    princessTowerBlueBottom = Tower({ VIRTUAL_WIDTH/9*2,BOTTOMPATHY }, DARKBLUE, PRINCESS);
    princessTowerRedTop = Tower({ VIRTUAL_WIDTH/9*7,TOPPATHY }, RED, PRINCESS);
    princessTowerRedBottom = Tower({ VIRTUAL_WIDTH/9*7,BOTTOMPATHY }, RED, PRINCESS);

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

    for (auto& troop : troops) {
        troop.draw();
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

void Arena::update(float dt) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 virtualMousePos = { GetMousePosition().x/GetScreenWidth()*VIRTUAL_WIDTH, GetMousePosition().y/GetScreenHeight()*VIRTUAL_HEIGHT };

        if (virtualMousePos.x < VIRTUAL_WIDTH/2.0f) {
            troops.push_back(Troop(virtualMousePos, DARKBLUE, KNIGHT));
        } else {
            troops.push_back(Troop(virtualMousePos, RED, MUSKETEER));
        }
    }

    //Show screen middle for debugging
    if (IsKeyPressed(KEY_F1)) {
        isMiddleDrawn = not isMiddleDrawn;
    }

    //Iterate through all troops and update them
    for (auto& troop : troops) {
        troop.update(dt, getTowerRecs(), troops);
    }

    troops.erase(
        std::remove_if(troops.begin(), troops.end(), [](const Troop& troop) {
            return troop.isDead();
        }),
        troops.end()
    );
}

Troop::Troop(Vector2 position, Color color, TroopType troopType) {
    pos = position;
    team = color;
    type = troopType;
    isEnemyInRange = false;
    isAttacking = false;
    currentFrame = 0;
    animTimer = 0.0f;

    switch (type) {
        case KNIGHT:
        maxHealth = 600.0f;
        health = maxHealth;
        attackDamage = 30.0f;
        attackRange = 40.0f;
        sightRange = 600.0f;
        speed = 150.0f;
        size = 50.0f;
        sprite = LoadTexture("../assets/knight.png");
        numFrames = 3;
        break;

        case MUSKETEER:
        maxHealth = 300.0f;
        health = maxHealth;
        attackDamage = 5.0f;
        attackRange = 200.0f;
        sightRange = 600.0f;
        speed = 150.0f;
        size = 40.0f;
        sprite = LoadTexture("../assets/musketeer.png");
        numFrames = 3;
        break;
    }
}

void Troop::update(float dt, const std::vector<Rectangle>& towerRecs, std::vector<Troop>& troops) {
    float step = speed * dt;

    Rectangle hitbox = getHitbox();

    isEnemyInRange = false;
    isAttacking = false;

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
        if (CheckCollisionRecs({hitbox.x+step, hitbox.y, hitbox.width, hitbox.height}, obstacle)) {
            blockedToRight = true;
        }
        if (CheckCollisionRecs({hitbox.x-step, hitbox.y, hitbox.width, hitbox.height}, obstacle)) {
            blockedToLeft = true;
        }
        if (CheckCollisionRecs({hitbox.x, hitbox.y+step, hitbox.width, hitbox.height}, obstacle)) {
            blockedBelow = true;
        }
        if (CheckCollisionRecs({hitbox.x, hitbox.y-step, hitbox.width, hitbox.height}, obstacle)) {
            blockedAbove = true;
        }
    }

    Troop* closestEnemy = nullptr;
    bool foundEnemy = false;

    for (auto& troop : troops) {
        if (&troop == this || ColorIsEqual(team, troop.getTeam())) {
            continue;
        }

        if (CheckCollisionCircleRec(pos, sightRange/2.0f, troop.getHitbox())) {
            isEnemyInRange = true;

            if (!foundEnemy || getDistanceToTroop(troop)<getDistanceToTroop(*closestEnemy)) {
                closestEnemy = &troop;
                foundEnemy = true;
            }

            if (CheckCollisionCircleRec(pos, attackRange/2.0f, troop.getHitbox())) {
                isAttacking = true;
            }
        }
    }

    const float frameDuration = 0.2f;

    if (isAttacking) {
        animTimer += dt;
        if (animTimer >= frameDuration) {
            animTimer = 0.0f;
            currentFrame = (currentFrame == 1) ? 2 : 1;
        }
    } else {
        animTimer += dt;
        if (animTimer >= frameDuration) {
            animTimer = 0.0f;
            currentFrame = (currentFrame == 0) ? 1 : 0;
        }
    }
    
    if (isAttacking) {
        closestEnemy->takeDamage(attackDamage);
    }  else if (isEnemyInRange) {
        Vector2 directionToEnemy = { pos.x-closestEnemy->pos.x, pos.y-closestEnemy->pos.y };

        if (directionToEnemy.x < 0 && abs(directionToEnemy.x) > step && !blockedToRight) {
            pos.x += step;
        } else if (directionToEnemy.x > 0 && abs(directionToEnemy.x) > step && !blockedToLeft) {
            pos.x -= step;
        }
        if (directionToEnemy.y < 0 && abs(directionToEnemy.y) > step && !blockedBelow) {
            pos.y += step;
        } else if (directionToEnemy.y > 0 && abs(directionToEnemy.y) > step && !blockedAbove) {
            pos.y -= step;
        }
    }else {
        float distanceToTopPath = abs(pos.y-TOPPATHY);
        float distanceToBottomPath = abs(pos.y-BOTTOMPATHY);

        if (pos.y > VIRTUAL_HEIGHT/2.0f) {
            if (pos.y < BOTTOMPATHY && not blockedBelow && distanceToBottomPath > step) {
                pos.y += step;
            } else if (not blockedAbove && distanceToBottomPath > step) {
                pos.y -= step;
            }
            
        } else {
            if (pos.y < TOPPATHY && not blockedBelow && distanceToTopPath > step) {
                pos.y += step;
            } else if (not blockedAbove && distanceToTopPath > step) {
                pos.y -= step;
            }
        }
        
        if (ColorIsEqual(team, DARKBLUE) && not blockedToRight) {
            pos.x += step;
        } else if (ColorIsEqual(team, RED) && not blockedToLeft) {
            pos.x -= step;
        }
    }
}

Rectangle Troop::getHitbox() const {
    Rectangle hitbox = { pos.x-size/2, pos.y-size/2, size, size };
    return hitbox;
}

void Troop::draw() {
    float healthBarWidth = 80.0f;
    Rectangle healthbarRed = { pos.x - healthBarWidth/2.0f, pos.y-50.0f, healthBarWidth, 10.0f };
    DrawRectangleRec(healthbarRed, RED);

    float healthRemainingPercent = health/maxHealth;
    Rectangle healthBarGreen = { healthbarRed.x, healthbarRed.y, healthbarRed.width*healthRemainingPercent, healthbarRed.height };
    DrawRectangleRec(healthBarGreen, GREEN);

    float frameWidth = float(sprite.width) / numFrames;
    Rectangle source = { currentFrame * frameWidth, 0.0f, frameWidth, float(sprite.height) };
    Rectangle dest = { roundf(pos.x-size/2), roundf(pos.y-size/2), size, size };
    DrawTexturePro(sprite, source, dest, {0,0}, 0.0f, team);
}

float Troop::getDistanceToTroop(Troop otherTroop) const {
    return Vector2Distance(pos, otherTroop.pos);
}