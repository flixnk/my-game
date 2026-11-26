#ifndef ARENA_H
#define ARENA_H
#include "raylib.h"
#include <vector>

constexpr float VIRTUAL_WIDTH = 2560.0f;
constexpr float VIRTUAL_HEIGHT = 1440.0f;
constexpr float TOPPATHY = 226.0f;
constexpr float BOTTOMPATHY = VIRTUAL_HEIGHT-TOPPATHY;

enum TowerType { PRINCESS = 0, KING = 1 };

enum TroopType { KNIGHT = 0, MUSKETEER = 2 };

class Tower {
    public:
    Tower() = default;
    Tower(Vector2 position, float dimensions, Color color, TowerType towerType);
    ~Tower();
    void draw();
    Rectangle getHitbox() const;

    private:
    Vector2 pos;
    int health;
    Color team;
    TowerType type;
    float size;
};

class Arena;

class Troop {
    public:
    Troop() = default;
    Troop(Vector2 position, Color color, TroopType troopType);
    ~Troop() = default;
    void draw();
    void move(const std::vector<Rectangle>& towerRecs, const std::vector<Troop>& troops);
    Rectangle getHitbox() const;
    Color getTeam() const { return team; }

    private:
    Vector2 pos;
    int health;
    Color team;
    float speed;
    float size;
    float attackRange;
    float sightRange;
    Texture2D sprite;
    TroopType type;
    bool isEnemyInRange;
};

class Arena {
    public:
    Arena();
    ~Arena();
    void draw();
    std::vector<Rectangle> getTowerRecs() const;
    std::vector<Rectangle> getTroopRecs() const;

    private:
    Texture2D arena;
    Tower kingTowerBlue;
    Tower kingTowerRed;
    Tower princessTowerBlueTop;
    Tower princessTowerBlueBottom;
    Tower princessTowerRedTop;
    Tower princessTowerRedBottom;
    std::vector<Troop> troops;
    bool isMiddleDrawn;
    std::vector<Tower> towers;
};

#endif