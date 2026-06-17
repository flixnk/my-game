#include "screenElements.h"

Button::Button(const char *fileName, float x, float y, float scale) : scale(scale) {
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

TextFieldNumbers::TextFieldNumbers(float x, float y, float width, float height, int maxSymbols) : x(x), y(y), width(width), height(height), maxSymbols(maxSymbols) {
  isFPSBoundsClicked = false;
}

TextFieldNumbers::~TextFieldNumbers() {}

int TextFieldNumbers::draw() {
  Rectangle fpsBounds = { x, y, width, height };

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(GetMousePosition(), fpsBounds)) {
      isFPSBoundsClicked = true;
    } else {
      isFPSBoundsClicked = false;
    }
  }
  
  int key = GetCharPressed();

  while (isFPSBoundsClicked && key > 0) {
    if ((key >= '0') && (key <= '9') && inputText.length() < maxSymbols) {
      inputText += (char)key;
    }
    key = GetCharPressed();
  }

  if (IsKeyPressed(KEY_BACKSPACE) && !inputText.empty()) {
    inputText.pop_back();
  }

  Color boxColor = isFPSBoundsClicked ? GRAY : DARKGRAY;
  DrawRectangleRec(fpsBounds, boxColor);

  DrawText(inputText.c_str(), fpsBounds.x+5, fpsBounds.y+2, fpsBounds.height-4, LIGHTGRAY);

  if (!inputText.empty()) {
    return std::stoi(inputText);
  }

  return -1;
}