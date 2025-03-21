#include "Keyboard.h"
#include "SDL3/SDL_events.h"

MoveAxis Keyboard::get_move() {
  move.x = (wasd & KEY_D ? 1 : 0) - (wasd & KEY_A ? 1 : 0);
  move.y = (wasd & KEY_S ? 1 : 0) - (wasd & KEY_W ? 1 : 0);
  return move;
}

void Keyboard::feed(SDL_Event &event) {

  // Bitwise toggle on 0-3 for WASD keydown
  if (event.type == SDL_EVENT_KEY_DOWN) {
    if (event.key.key == SDLK_W)
      wasd |= KEY_W;
    if (event.key.key == SDLK_A)
      wasd |= KEY_A;
    if (event.key.key == SDLK_S)
      wasd |= KEY_S;
    if (event.key.key == SDLK_D)
      wasd |= KEY_D;
  }

  // Bitwise toggle off 0-3 for WASD keyup
  if (event.type == SDL_EVENT_KEY_UP) {
    if (event.key.key == SDLK_W)
      wasd &= ~KEY_W;
    if (event.key.key == SDLK_A)
      wasd &= ~KEY_A;
    if (event.key.key == SDLK_S)
      wasd &= ~KEY_S;
    if (event.key.key == SDLK_D)
      wasd &= ~KEY_D;
  }
}
