#include "hud.h"
#include "arena.h"
#include "raylib.h"

Hud::Hud() {}

Hud::~Hud() {}

void Hud::draw() {
    Vector2 startPos = { 135, 86 };
    int rectangleDistance = 58;
    for (int i = 0; i<VIRTUAL_WIDTH/2.0f-30.0f; i++) {
        for (int j = 0; j<VIRTUAL_HEIGHT-startPos.y-290.0f; j++) {
            if (i % rectangleDistance == 0 && j % rectangleDistance == 0) {
                //DrawRectangle(startPos.x+j, startPos.y+i, rectangleDistance-5.0f, rectangleDistance-5.0f, Fade(WHITE, 0.2f));
            }
        }
    }
}