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
    Tower(Vector2 position, Vector2 dimensions, Color color, TowerType towerType);
    ~Tower();
    Rectangle hitbox;
    void draw();

    private:
    Vector2 pos;
    int health;
    Color team;
    TowerType type;
    Vector2 size;
    
};

class Arena;

class Troop {
    public:
    Troop() = default;
    Troop(Vector2 position, Color color, TroopType troopType);
    ~Troop() = default;
    void draw();
    void move(const std::vector<Rectangle>& towerRecs);
    Rectangle hitbox;

    private:
    Vector2 pos;
    int health;
    Color team;
    float speed;
    float troopSize;
    float attackRange;
    float sightRange;
    Texture2D sprite;
    TroopType type;
};

class Arena {
    public:
    Arena();
    ~Arena();
    void draw();
    std::vector<Rectangle> getTowerRecs() const;

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