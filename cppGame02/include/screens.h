#ifndef SCREENS_H
#define SCREENS_H

#include "raylib.h"
class Screens {
public:
  virtual ~Screens();
  virtual void draw() = 0;

protected:
};

class OptionsScreen : public Screens {
public:
  OptionsScreen();
  ~OptionsScreen();

  void draw() override;

private:
  Texture2D optionsButtonSprite;
  Texture2D startButtonSprite;
  Texture2D creditsButtonSprite;
};

class WinScreen : public Screens {
public:
  WinScreen();
  ~WinScreen();

  void draw() override;
};

class LossScreen : public Screens {
public:
  LossScreen();
  ~LossScreen();

  void draw() override;
};

#endif
