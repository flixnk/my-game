#include "screens.h"
#include "raylib.h"
#include "screenElements.h"
#include "utilities.h"

Screens::~Screens() {}

MenuScreen::MenuScreen() {
  float scale = 5.0f;

  startButton = new Button("start-button.png", 0, 0, scale);
  optionsButton = new Button("options-button.png", 0, 0, scale);
  creditsButton = new Button("credits-button.png", 0, 0, scale);

  updateLayout();
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
  if (IsWindowResized()) {
    updateLayout();
  }

  if (startButton->isClicked()) {
    return GAME;
  } else if (optionsButton->isClicked()) {
    return OPTIONS;
  } else if (creditsButton->isClicked()) {
    return CREDITS;
  }
  return MENU;
}

void MenuScreen::updateLayout() {
  int spacer = 125;
  float centerX = GetScreenWidth() / 2.0f;
  float startY = GetScreenHeight() - spacer*4;

  startButton->setPosition(centerX, startY);
  optionsButton->setPosition(centerX, startY + spacer);
  creditsButton->setPosition(centerX, startY + spacer * 2);
}

WinScreen::WinScreen() {}

WinScreen::~WinScreen() {}

void WinScreen::draw() { ClearBackground(GREEN); }

ScreenType WinScreen::update() {
  if (IsKeyPressed(KEY_ENTER)) {
    return MENU;
  }
  return WIN;
}

void WinScreen::updateLayout() {}

LossScreen::LossScreen() {}

LossScreen::~LossScreen() {}

void LossScreen::draw() { ClearBackground(RED); }

ScreenType LossScreen::update() {
  if (IsKeyPressed(KEY_ENTER)) {
    return MENU;
  }
  return LOSS;
}

void LossScreen::updateLayout() {}

OptionsScreen::OptionsScreen() : fpsField(100, 45, 532, 28, 4), slider(100, 90, 357, 86) {
  float scale = 5.0f;
  backButton = new Button("back-button.png", 0, 0, scale);
  screenSizeButton = new Button("placehold-button.png", 0, 0, scale);

  updateLayout();
}

OptionsScreen::~OptionsScreen() {
  delete backButton;
  delete screenSizeButton;
}

void OptionsScreen::draw() { 
  ClearBackground(GRAY);
  backButton->draw();
  screenSizeButton->draw();
  
  int inputText = fpsField.draw();
  int sliderNumber = slider.draw();

  if (IsKeyPressed(KEY_ENTER)) {
    int newFPS = inputText;
    if (newFPS > 0 && newFPS < 20) {
      newFPS = 20;
    }
    SetTargetFPS(newFPS);
  }
 }

ScreenType OptionsScreen::update() {
  if (IsWindowResized()) {
    updateLayout();
  }
  if (backButton->isClicked()) {
    return MENU;
  }
  if (screenSizeButton->isClicked()) {
    if (IsWindowMaximized()) {
      RestoreWindow();
    } else {
      MaximizeWindow();
    }
    updateLayout();
  }
  return OPTIONS;
}

void OptionsScreen::updateLayout() {
  float rightVert = GetScreenWidth() - (float)GetScreenWidth()/3;
  float leftVert = GetScreenWidth() - (float)GetScreenWidth()/3*2;
  backButton->setPosition(rightVert, 100);
  screenSizeButton->setPosition(leftVert, 100);
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

void CreditsScreen::updateLayout() {}

ScreenType CreditsScreen::update() {
  if (backButton->isClicked()) {
    return MENU;
  }
  return CREDITS;
}

