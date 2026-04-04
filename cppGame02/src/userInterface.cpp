#include "userInterface.h"

UserInterface::UserInterface() {
    heartSprite = LoadTexture("assets/heart.png");
    SetTextureFilter(heartSprite, TEXTURE_FILTER_POINT);

    if (heartSprite.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load heart png");
    }
    heartPos = { 20, 20 };
}

UserInterface::~UserInterface() {
    UnloadTexture(heartSprite);
}

void UserInterface::draw(int heartsRemaining) {
    if (heartsRemaining > 0) {
        DrawTexture(heartSprite, heartPos.x, heartPos.y, WHITE);
    } else {
        DrawTexture(heartSprite, heartPos.x, heartPos.y, BLACK);
    }
    if (heartsRemaining > 1) {
        DrawTexture(heartSprite, heartPos.x+40, heartPos.y, WHITE);
    } else {
        DrawTexture(heartSprite, heartPos.x+40, heartPos.y, BLACK);
    }
    if (heartsRemaining > 2) {
        DrawTexture(heartSprite, heartPos.x+80, heartPos.y, WHITE);
    } else {
        DrawTexture(heartSprite, heartPos.x+80, heartPos.y, BLACK);
    }
}