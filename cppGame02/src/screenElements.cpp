#include "screenElements.h"

Button::Button(const char *fileName, float x, float y, float scale) : scale(scale) {
  sprite = LoadTexture(GetAssetPath(fileName));
  SetTextureFilter(sprite, TEXTURE_FILTER_POINT);
  posX = x;
  posY = y;
  isHovered = false;
}
Button::~Button() { 
  UnloadTexture(sprite); 
}

void Button::draw() {
  bounds = {posX, posY, sprite.width*scale, sprite.height*scale};

  isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);

  Color tint = isHovered ? GRAY : WHITE;

  DrawTextureEx(sprite, {bounds.x, bounds.y}, 0.0f, scale, tint);
}

bool Button::isClicked() {
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

  Color boxColor = isFPSBoundsClicked ? LIGHTGRAY : DARKGRAY;
  DrawRectangleRec(fpsBounds, boxColor);

  DrawText(inputText.c_str(), fpsBounds.x+5, fpsBounds.y+2, fpsBounds.height-4, GRAY);

  if (!inputText.empty()) {
    return std::stoi(inputText);
  }

  return -1;
}

SliderPercentage::SliderPercentage(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {
  sliderText = "";
  isSliderBoundsClicked = false;
  sliderPos = { x, y };
}

SliderPercentage::~SliderPercentage() {}

int SliderPercentage::draw() {
  Rectangle slider = { sliderPos.x, sliderPos.y, width/15, height };
  Rectangle sliderBackground = { x, y, width+slider.width, height };

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (CheckCollisionPointRec(GetMousePosition(), sliderBackground)) {
      isSliderBoundsClicked = true;
    } else {
      isSliderBoundsClicked = false;
    }
  }

  if (isSliderBoundsClicked) {
    sliderPos.x = GetMousePosition().x;
    if (sliderPos.x < sliderBackground.x) {
      sliderPos.x = sliderBackground.x;
    } else if (sliderPos.x > sliderBackground.x + sliderBackground.width-slider.width) {
      sliderPos.x = sliderBackground.x + sliderBackground.width-slider.width;
    }
  }

  DrawRectangleRec(sliderBackground, GREEN);
  DrawRectangleRec(slider, RED);
  
  if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
    isSliderBoundsClicked = false;
  }

  int sliderNumber = (sliderPos.x - x) / (sliderBackground.width - slider.width) * 100.0f;
  
  sliderText = std::to_string(sliderNumber)+'%';
  DrawText(sliderText.c_str(), x+width/2, y+height/4, height/2, BLACK);

  return sliderNumber;
}