#ifndef ARENA_H
#define ARENA_H
#include "raylib.h"
#include <vector>

constexpr float VIRTUAL_WIDTH = 2560.0f;
constexpr float VIRTUAL_HEIGHT = 1440.0f;
constexpr float TOPPATHY = 226.0f;
constexpr float BOTTOMPATHY = VIRTUAL_HEIGHT-TOPPATHY;

enum TroopType { PRINCESS_TOWER = 0, KING_TOWER = 1, KNIGHT = 2, MUSKETEER = 3 };

class Arena;

class Troop {
    public:
    Troop() = default;
    Troop(Vector2 position, Color color, TroopType troopType);
    ~Troop() = default;
    void draw();
    void update(float dt, std::vector<Troop>& troops);
    Rectangle getHitbox() const;
    Color getTeam() const { return team; }
    float getHealth() const { return health; }
    float getDistanceToTroop(Troop otherTroop) const;
    bool isDead() const { return health <= 0.0f; }
    void takeDamage(float damage) { health -= damage; }

    private:
    Vector2 pos;
    float maxHealth;
    float health;
    Color team;
    float speed;
    float size;
    float attackRange;
    float sightRange;
    Texture2D sprite;
    TroopType type;
    bool isEnemyInRange;
    bool isAttacking;
    int currentFrame;
    float animTimer;
    int numFrames;
    float attackDamage;
};

class Arena {
    public:
    Arena();
    ~Arena();
    void draw();
    std::vector<Rectangle> getTroopRecs() const;
    void update(float dt);

    private:
    Texture2D arena;
    Troop kingTowerBlue;
    Troop kingTowerRed;
    Troop princessTowerBlueTop;
    Troop princessTowerBlueBottom;
    Troop princessTowerRedTop;
    Troop princessTowerRedBottom;
    std::vector<Troop> troops;
    bool isMiddleDrawn;
};

#endif