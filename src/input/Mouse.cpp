#include "Mouse.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>

void Mouse::tick() {

  // Grab the buttons and mouse coords
  auto buttons = SDL_GetMouseState(&mx_, &my_);

  // Where were we last tick?
  last_left = left;
  last_middle = middle;
  last_right = right;

  // Where are we this tick?
  left = buttons & (1 << 0);
  middle = buttons & (1 << 1);
  right = buttons & (1 << 2);
}

// Is the cursor within a rect?
bool Mouse::over(int x, int y, int w, int h) {
  return mx_ >= x && my_ >= y && mx_ <= x + w && my_ <= y + h;
}

// Is a given mouse button down?
bool Mouse::down(BTN btn) {
  switch (btn) {
  case LEFT:
    return left;
  case MIDDLE:
    return middle;
  case RIGHT:
    return right;
  }
}

// Was a given mouse button down last tick, but not this one (aka, clicked)?
bool Mouse::clicked(BTN btn) {
  switch (btn) {
  case LEFT:
    return last_left && !left;
  case MIDDLE:
    return last_middle && !middle;
  case RIGHT:
    return last_right && !right;
  }
}
