#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "raylib.h"

class UserInterface {
public:
  UserInterface();
  ~UserInterface();

  void draw(int heartsRemaining);

private:
  Texture2D heartSprite;
  Vector2 heartPos;
};

#endif
