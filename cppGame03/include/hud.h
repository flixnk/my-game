#ifndef HUD_H
#define HUD_H
#include "raylib.h"
#include <vector>

class Hud {
    public:
    Hud();
    ~Hud();
    void draw();

    private:
    std::vector<Rectangle> placableRecs;
};

#endif