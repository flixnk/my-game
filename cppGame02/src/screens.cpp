#include "screens.h"
#include "raylib.h"
#include "utilities.h"

Screens::~Screens() {}

MenuScreen::MenuScreen() {
  float centerX = GetScreenWidth() / 2.0f;
  float startY = 450.0f;
  float scale = 5.0f;

  startButton = new Button("start-button.png", centerX, startY, scale);
  optionsButton = new Button("options-button.png", centerX, startY + 125, scale);
  creditsButton = new Button("credits-button.png", centerX, startY + 250, scale);
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
  } else if (creditsButton->isClicked()) {
    return CREDITS;
  }
  return MENU;
}
WinScreen::WinScreen() {}

WinScreen::~WinScreen() {}

void WinScreen::draw() { ClearBackground(GREEN); }

ScreenType WinScreen::update() {
  return WIN;
}

LossScreen::LossScreen() {}

LossScreen::~LossScreen() {}

void LossScreen::draw() { ClearBackground(RED); }

ScreenType LossScreen::update() {
  return LOSS;
}

OptionsScreen::OptionsScreen() {
  float scale = 5.0f;
  float rightVert = GetScreenWidth() - (float)GetScreenWidth()/3;
  backButton = new Button("back-button.png", rightVert, 100.0f, scale);
}

OptionsScreen::~OptionsScreen() {
  delete backButton;
}

void OptionsScreen::draw() { 
  ClearBackground(PURPLE);
  backButton->draw();
 }

ScreenType OptionsScreen::update() {
  if (backButton->isClicked()) {
    return MENU;
  }
  return OPTIONS;
}

CreditsScreen::CreditsScreen() {
  float scale = 5.0f;
  float rightVert = GetScreenWidth() - (float)GetScreenWidth()/3;
  backButton = new Button("back-button.png", rightVert, 100.0f, scale);

  creditsName = LoadTexture("assets/credits-name.png");
  SetTextureFilter(creditsName, TEXTURE_FILTER_POINT);

  bonkSound = LoadSound("assets/Bonk-Sound-Effect.mp3");

  curX = 1;
  curY = 1;
  directionX = 2;
  directionY = 2;
}

CreditsScreen::~CreditsScreen() {
  UnloadTexture(creditsName);
  delete backButton;
  UnloadSound(bonkSound);
}

void CreditsScreen::draw() {
  ClearBackground(ORANGE);
  backButton->draw();

  float scale = 3.0f;

  DrawTextureEx(creditsName, {(float)curX, (float)curY}, 0.0f, scale, WHITE);

  if ((curX+creditsName.width*scale) >= GetScreenWidth() || curX <= 0) {
    directionX *= -1;
    PlaySound(bonkSound);
  }
  if ((curY+creditsName.height*scale) >= GetScreenHeight() || curY <= 0) {
    directionY *= -1;
    PlaySound(bonkSound);
  }
  curX += directionX;
  curY += directionY;
}

ScreenType CreditsScreen::update() {
  if (backButton->isClicked()) {
    return MENU;
  }
  return CREDITS;
}

Button::Button(const char *fileName, float x, float y, float scale)
    : scale(scale) {
  sprite = LoadTexture(GetAssetPath(fileName));
  SetTextureFilter(sprite, TEXTURE_FILTER_POINT);
  posX = x;
  posY = y;
  isHovered = false;
  clickSound = LoadSound("assets/Mouse-Click-Sound-Effect.mp3");
}
Button::~Button() { 
  UnloadTexture(sprite); 
  UnloadSound(clickSound);
}

void Button::draw() {
  float currentCenterX = GetScreenWidth() / 2.0f;

  float scaledWidth = sprite.width * scale;
  float scaledHeight = sprite.height * scale;

  float finalX = currentCenterX - (scaledWidth / 2.0f);

  bounds = {finalX, posY, scaledWidth, scaledHeight};


  isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);

  Color tint = isHovered ? GRAY : WHITE;

  DrawTextureEx(sprite, {bounds.x, bounds.y}, 0.0f, scale, tint);
}

bool Button::isClicked() {
  PlaySound(clickSound);
  return CheckCollisionPointRec(GetMousePosition(), bounds) &&
         IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
