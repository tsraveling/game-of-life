
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "SDL3/SDL_events.h"
struct MoveAxis {
  float x;
  float y;
};

#define KEY_W (1 << 0)
#define KEY_A (1 << 1)
#define KEY_S (1 << 2)
#define KEY_D (1 << 3)

class Keyboard {
private:
  unsigned char wasd;
  MoveAxis move{0, 0};

public:
  MoveAxis get_move();
  void feed(SDL_Event &event);
};

#endif // KEYBOARD_H
