#ifndef SCREENS_H
#define SCREENS_H

#include "raylib.h"
#include "utilities.h"
#include "screenElements.h"

class Screens {
public:
  virtual ~Screens();
  virtual void draw() = 0;

  virtual ScreenType update() = 0;

protected:
  virtual void updateLayout() = 0;
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

private:
  void updateLayout() override;
};

class WinScreen : public Screens {
public:
  WinScreen();
  ~WinScreen();

  void draw() override;
  ScreenType update() override;

private:
  void updateLayout() override;
};

class LossScreen : public Screens {
public:
  LossScreen();
  ~LossScreen();

  void draw() override;
  ScreenType update() override;

private:
  void updateLayout() override;
};

class OptionsScreen : public Screens {
  Button *backButton;
  Button *screenSizeButton;
public:
  OptionsScreen();
  ~OptionsScreen();

  void draw() override;
  ScreenType update() override;
private:
  TextFieldNumbers fpsField;
  SliderPercentage slider;

  void updateLayout() override;
};

class CreditsScreen : public Screens {
  Button *backButton;
public:
  CreditsScreen();
  ~CreditsScreen();

  Texture2D creditsName;
  Sound bonkSound;
  int curX;
  int curY;
  int directionX;
  int directionY;

  void draw() override;
  ScreenType update() override;

private:
  void updateLayout() override;
};

#endif
