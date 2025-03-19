
#ifndef MOUSE_H
#define MOUSE_H

enum BTN { LEFT, MIDDLE, RIGHT };

class Mouse {

  bool last_left, last_middle, last_right = false;
  bool left, middle, right = false;
  float mx_, my_ = 0;

public:
  float mx() const {
    return mx_;
  } // Read-only return of mx and my (can't set elsewhere)
  float my() const { return my_; }

  /**
   * Processes input, collects events, etc.
   */
  void tick();

  /**
   * Returns true if the mouse is over a given rect
   * @param x,y upper left coords
   * @param w,h size
   */
  bool over(int x, int y, int w, int h);

  /**
   * Returns if a mouse button is being held down
   */
  bool down(BTN btn = LEFT);

  /**
   * Returns is a mouse button was pressed and released
   */
  bool clicked(BTN btn = LEFT);
};

#endif // MOUSE_H
