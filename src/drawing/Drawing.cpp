#include "Drawing.h"
#include <SDL3/SDL_render.h>

namespace Colors {
const Color BLACK{0, 0, 0, 255};
const Color WHITE{255, 255, 255, 255};
const Color GRAY{128, 128, 128, 255};
const Color RED{255, 0, 0, 255};
} // namespace Colors

void Draw::rect(float x, float y, float w, float h) {
  SDL_FRect rect = {x, y, w, h};
  SDL_RenderFillRect(renderer, &rect);
}

void Draw::set_color(const Color &color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Draw::set_color(unsigned char r, unsigned char g, unsigned char b) {
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}
void Draw::set_color(unsigned char r, unsigned char g, unsigned char b,
                     unsigned char a) {

  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
