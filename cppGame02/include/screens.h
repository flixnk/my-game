#ifndef SCREENS_H
#define SCREENS_H

#include "raylib.h"
#include "utilities.h"

inline const char *GetAssetPath(const char *fileName) {
  return TextFormat("../assets/%s", fileName);
}

class Button {
public:
  Texture2D sprite;
  Rectangle bounds;
  float scale;

  Button(const char *fileName, float x, float y, float scale);
  ~Button();

  void draw();
  bool isClicked();

private:
  float posX;
  float posY;
};
class Screens {
public:
  virtual ~Screens();
  virtual void draw() = 0;

  virtual ScreenType update() = 0;

protected:
};

class MenuScreen : public Screens {
  Button *startButton;
  Button *optionsButton;
  Button *creditsButton;

public:
  MenuScreen();
  ~MenuScreen();

  void draw() override;
  ScreenType update() override;
};

class WinScreen : public Screens {
public:
  WinScreen();
  ~WinScreen();

  void draw() override;
  ScreenType update() override;
};

class LossScreen : public Screens {
public:
  LossScreen();
  ~LossScreen();

  void draw() override;
  ScreenType update() override;
};

class OptionsScreen : public Screens {
public:
  OptionsScreen();
  ~OptionsScreen();

  void draw() override;
  ScreenType update() override;
};

#endif
