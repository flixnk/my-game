#include "screens.h"
#include "raylib.h"
#include "utilities.h"

Screens::~Screens() {}

MenuScreen::MenuScreen() {
  float centerX = GetScreenWidth() / 2.0f;
  float startY = 450.0f;
  float scale = 5.0f;

  startButton = new Button("start-button.png", centerX, startY, scale);
  optionsButton =
      new Button("options-button.png", centerX, startY + 125, scale);
  creditsButton =
      new Button("credits-button.png", centerX, startY + 250, scale);
}

MenuScreen::~MenuScreen() {
  delete startButton;
  delete optionsButton;
  delete creditsButton;
}

void MenuScreen::draw() {
  ClearBackground(GRAY);
  startButton->draw();
  optionsButton->draw();
  creditsButton->draw();
}

ScreenType MenuScreen::update() {
  if (startButton->isClicked()) {
    return GAME;
  } else if (optionsButton->isClicked()) {
    return OPTIONS;
  }
  return MENU;
}
WinScreen::WinScreen() {}

WinScreen::~WinScreen() {}

void WinScreen::draw() { ClearBackground(GREEN); }

ScreenType WinScreen::update() {}

LossScreen::LossScreen() {}

LossScreen::~LossScreen() {}

void LossScreen::draw() { ClearBackground(RED); }

ScreenType LossScreen::update() {}

OptionsScreen::OptionsScreen() {}

OptionsScreen::~OptionsScreen() {}

void OptionsScreen::draw() { ClearBackground(PURPLE); }

ScreenType OptionsScreen::update() {}

Button::Button(const char *fileName, float x, float y, float scale)
    : scale(scale) {
  sprite = LoadTexture(GetAssetPath(fileName));
  SetTextureFilter(sprite, TEXTURE_FILTER_POINT);
  posX = x;
  posY = y;
  isHovered = false;
}
Button::~Button() { UnloadTexture(sprite); }

void Button::draw() {
  float currentCenterX = GetScreenWidth() / 2.0f;

  float scaledWidth = sprite.width * scale;
  float scaledHeight = sprite.height * scale;

  float finalX = currentCenterX - (scaledWidth / 2.0f);

  bounds = {finalX, posY, scaledWidth, scaledHeight};

  if (isHovered) {
    DrawTextureEx(sprite, {bounds.x, bounds.y}, 0.0f, scale, GRAY);
  } else {
    DrawTextureEx(sprite, {bounds.x, bounds.y}, 0.0f, scale, WHITE);
  }
}

bool Button::isClicked() {
  if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
    isHovered = true;
  } else {
    isHovered = false;
  }
  return CheckCollisionPointRec(GetMousePosition(), bounds) &&
         IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
