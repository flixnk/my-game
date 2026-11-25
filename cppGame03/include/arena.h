#ifndef ARENA_H
#define ARENA_H
#include "raylib.h"

enum TowerType { PRINCESS = 0, KING = 1 };

class Arena {
    public:
    Arena();
    ~Arena();
    void draw();

    private:
    Texture2D arena;
};

class Tower {
    public:
    Tower(Vector2 position, Vector2 dimensions, Color color, TowerType towerType);
    ~Tower();
    void draw();

    private:
    Vector2 pos;
    int health;
    Color team;
    TowerType type;
    Vector2 size;
};

#endif