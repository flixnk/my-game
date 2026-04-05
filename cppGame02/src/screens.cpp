#include "screens.h"
#include "raylib.h"

Screens::~Screens() {}

OptionsScreen::OptionsScreen() {
  startButtonSprite = LoadTexture("../assets/start-button.png");
  SetTextureFilter(startButtonSprite, TEXTURE_FILTER_POINT);
  if (startButtonSprite.id == 0) {
    TraceLog(LOG_ERROR, "Failed to load start button png.");
  }

  optionsButtonSprite = LoadTexture("../assets/options-button.png");
  SetTextureFilter(optionsButtonSprite, TEXTURE_FILTER_POINT);
  if (optionsButtonSprite.id == 0) {
    TraceLog(LOG_ERROR, "Failed to load options button png.");
  }

  creditsButtonSprite = LoadTexture("../assets/credits-button.png");
  SetTextureFilter(creditsButtonSprite, TEXTURE_FILTER_POINT);
  if (creditsButtonSprite.id == 0) {
    TraceLog(LOG_ERROR, "Failed to load credits button png.");
  }
}

OptionsScreen::~OptionsScreen() {
  UnloadTexture(startButtonSprite);
  UnloadTexture(optionsButtonSprite);
  UnloadTexture(creditsButtonSprite);
}

void OptionsScreen::draw() {
  ClearBackground(GRAY);

  float buffer = 125.0f;
  float scale = 5.0f;
  float centerX = GetScreenWidth() / 2.0f;
  float posY = GetScreenHeight() - buffer * 4;

  Vector2 startPos = {centerX - (startButtonSprite.width * scale) / 2.0f, posY};
  Vector2 optionsPos = {centerX - (optionsButtonSprite.width * scale) / 2.0f,
                        posY + buffer};
  Vector2 creditsPos = {centerX - (creditsButtonSprite.width * scale / 2.0f),
                        posY + buffer * 2.0f};

  DrawTextureEx(startButtonSprite, startPos, 0.0f, scale, WHITE);
  DrawTextureEx(optionsButtonSprite, optionsPos, 0.0f, scale, WHITE);
  DrawTextureEx(creditsButtonSprite, creditsPos, 0.0f, scale, WHITE);
}

WinScreen::WinScreen() {}

WinScreen::~WinScreen() {}

void WinScreen::draw() { ClearBackground(GREEN); }

LossScreen::LossScreen() {}

LossScreen::~LossScreen() {}

void LossScreen::draw() { ClearBackground(RED); }
