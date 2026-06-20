#ifndef SCREENELEMENTS_H
#define SCREENELEMENTS_H

#include "raylib.h"
#include <string>

inline const char *GetAssetPath(const char *fileName) {
  return TextFormat("../assets/%s", fileName);
}

class Button {
public:
  Button(const char *fileName, float x, float y, float scale);
  ~Button();
  Texture2D sprite;
  Rectangle bounds;
  float scale;

  void draw();
  bool isClicked();

  void setPosition(float newX, float newY);

private:
  float x;
  float y;
  bool isHovered;
};

class TextFieldNumbers {
public:
  TextFieldNumbers(float x, float y, float width, float height, int maxSymbols);
  ~TextFieldNumbers();

  float x;
  float y;
  float width;
  float height;
  int maxSymbols;

  int draw();

private:
  bool isFPSBoundsClicked;
  std::string inputText;
};

class SliderPercentage {
public:
 SliderPercentage(float x, float y, float width, float height);
 ~SliderPercentage();

 float x;
 float y;
 float width;
 float height;

  int draw();
private:
  std::string sliderText;
  bool isSliderBoundsClicked;
  Vector2 sliderPos;
};

class SliderWithFewerPositions {
public:
 SliderWithFewerPositions(float x, float y, float width, float height);
 ~SliderWithFewerPositions();

 float x;
 float y;
 float width;
 float height;

  int draw();
private:
  std::string sliderText;
  bool isSliderBoundsClicked;
  Vector2 sliderPos;
};

#endif