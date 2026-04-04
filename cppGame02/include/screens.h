#ifndef SCREENS_H
#define SCREENS_H

enum BlockType { MENU = 0, WIN = 1, LOSS = 2 };

class Screens {
public:
  Screens();
  ~Screens();

private:
  int whatScreen;
};

#endif
