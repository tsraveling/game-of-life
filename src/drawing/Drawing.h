#ifndef DRAWING_H
#define DRAWING_H

#include <SDL3/SDL.h>

struct Color {
  unsigned char r, g, b, a = 255;
  Color() = default;
  Color(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa)
      : r{rr}, g{gg}, b{bb}, a{aa} {}
};

namespace Colors {
extern const Color BLACK;
extern const Color WHITE;
extern const Color GRAY;
extern const Color RED;
} // namespace Colors

class Draw {
private:
  SDL_Renderer *renderer;

public:
  /**
   * Draws a rectangle in the currently set rendering color
   * @param x,y origin point
   * @param w,h size
   */
  void rect(float x, float y, float w, float h);

  /**
   * Sets the current rendering color
   * @param color the current color
   */
  void set_color(const Color &color);
  void set_color(unsigned char r, unsigned char g, unsigned char b);
  void set_color(unsigned char r, unsigned char g, unsigned char b,
                 unsigned char a);

  /**
   * Prints some debug text on the screen
   * @param x,y coordinates
   * @param string Use this as a snprintf, e.g. `"Something %.2f", float_value`.
   * 128 char limit.
   */
  void dbg_print(float x, float y, const char *format, ...);

  Draw(SDL_Renderer *r) : renderer{r} {}
};

#endif // DRAWING_H
