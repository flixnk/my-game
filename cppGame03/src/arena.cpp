#include "arena.h"
#include "raylib.h"
#include <vector>
#include "raymath.h"
#include "utils.h"

Arena::Arena() {
    arena = LoadTexture("../assets/arena.png");

    kingTowerBlue = Troop({ VIRTUAL_WIDTH/9, VIRTUAL_HEIGHT/2 }, DARKBLUE, KING_TOWER);
    kingTowerRed = Troop({ VIRTUAL_WIDTH/9*8, VIRTUAL_HEIGHT/2 }, RED, KING_TOWER);
    princessTowerBlueTop = Troop({ VIRTUAL_WIDTH/9*2,TOPPATHY }, DARKBLUE, PRINCESS_TOWER);
    princessTowerBlueBottom = Troop({ VIRTUAL_WIDTH/9*2,BOTTOMPATHY }, DARKBLUE, PRINCESS_TOWER);
    princessTowerRedTop = Troop({ VIRTUAL_WIDTH/9*7,TOPPATHY }, RED, PRINCESS_TOWER);
    princessTowerRedBottom = Troop({ VIRTUAL_WIDTH/9*7,BOTTOMPATHY }, RED, PRINCESS_TOWER);

    isMiddleDrawn = false;

    troops.push_back(kingTowerBlue);
    troops.push_back(kingTowerRed);
    troops.push_back(princessTowerBlueTop);
    troops.push_back(princessTowerBlueBottom);
    troops.push_back(princessTowerRedTop);
    troops.push_back(princessTowerRedBottom);
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

    Hud hud;
    hud.draw();
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
        troop.update(dt, troops);
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
        case PRINCESS_TOWER:
        maxHealth = 2000.0f;
        health = maxHealth;
        attackDamage = 60.0f;
        attackRange = 650.0f;
        sightRange = attackRange;
        speed = 0.0f;
        size = 70.0f;
        sprite = LoadTexture("../assets/princessTower.png");
        numFrames = 3;
        break;

        case KING_TOWER:
        maxHealth = 4000.0f;
        health = maxHealth;
        attackDamage = 60.0f;
        attackRange = 650.0f;
        sightRange = attackRange;
        speed = 0.0f;
        size = 100.0f;
        sprite = LoadTexture("../assets/princessTower.png");
        numFrames = 3;
        break;

        case KNIGHT:
        maxHealth = 600.0f;
        health = maxHealth;
        attackDamage = 300.0f;
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
        attackDamage = 100.0f;
        attackRange = 200.0f;
        sightRange = 600.0f;
        speed = 150.0f;
        size = 40.0f;
        sprite = LoadTexture("../assets/musketeer.png");
        numFrames = 3;
        break;
    }
}

void Troop::update(float dt, std::vector<Troop>& troops) {
    float step = speed * dt;

    Rectangle hitbox = getHitbox();

    isEnemyInRange = false;
    isAttacking = false;

    bool blockedToRight = false;
    bool blockedToLeft = false;
    bool blockedAbove = false;
    bool blockedBelow = false;
    
    std::vector<Rectangle> obstacles;

    for (auto& troop : troops) {
        if (&troop == this) {
            continue;
        }
        obstacles.push_back(troop.getHitbox());
    }

    obstacles.push_back(RIVER_MIDDLE);
    obstacles.push_back(RIVER_BOTTOM);
    obstacles.push_back(RIVER_TOP);

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

    for (auto& troop : troops) {
        if (&troop == this || ColorIsEqual(team, troop.getTeam())) {
            continue;
        }

        if (CheckCollisionCircleRec(pos, sightRange, troop.getHitbox())) {
            isEnemyInRange = true;

            if (closestEnemy == nullptr || getDistanceToTroop(troop)<getDistanceToTroop(*closestEnemy)) {
                closestEnemy = &troop;
            }

            if (CheckCollisionCircleRec(pos, attackRange, troop.getHitbox())) {
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
        float actualDamage = attackDamage*dt;
        closestEnemy->takeDamage(actualDamage);
    }  else if (isEnemyInRange && !isRiverBetween(*closestEnemy)) {
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
    Rectangle healthbarTop = { pos.x - healthBarWidth/2.0f, pos.y-size*0.8f, healthBarWidth, 10.0f };
    DrawRectangleRec(healthbarTop, DARKGRAY);

    float healthRemainingPercent = health/maxHealth;
    Rectangle healthBarBelow = { healthbarTop.x, healthbarTop.y, healthbarTop.width*healthRemainingPercent, healthbarTop.height };
    DrawRectangleRec(healthBarBelow, team);

    if (health != maxHealth) {
        DrawText(TextFormat("%.0f", health), healthBarBelow.x+2.0f, healthBarBelow.y+1.0f, 10.0f, BLACK);
    }
    
    float frameWidth = float(sprite.width) / numFrames;
    float drawWidth = ColorIsEqual(team, RED) ? -frameWidth : frameWidth;
    Rectangle source = { currentFrame * frameWidth, 0.0f, drawWidth, float(sprite.height) };
    Rectangle dest = { roundf(pos.x-size/2), roundf(pos.y-size/2), size, size };
    DrawTexturePro(sprite, source, dest, {0,0}, 0.0f, team);
}

float Troop::getDistanceToTroop(Troop otherTroop) const {
    return Vector2Distance(pos, otherTroop.pos);
}

bool Troop::isRiverBetween(Troop otherTroop) {
    bool isTopRiverBetween = CheckCollisionLineRec(pos, otherTroop.getPosition(), RIVER_TOP);
    bool isMiddleRiverBetween = CheckCollisionLineRec(pos, otherTroop.getPosition(), RIVER_MIDDLE);
    bool isBottomRiverBetween = CheckCollisionLineRec(pos, otherTroop.getPosition(), RIVER_BOTTOM);

    return isTopRiverBetween || isMiddleRiverBetween || isBottomRiverBetween;
}