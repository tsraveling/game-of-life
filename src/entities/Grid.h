
#ifndef GRID_H
#define GRID_H

#include <vector>
class Grid {

private:
  std::vector<bool> map;
  int w, h = 64;

  /**
   * Counts how many live neighbors are at the given index
   */
  int count_neighbors(int x, int y);

  /**
   * Checks an x,y offset from a given index
   */
  bool check(int at_x, int at_y, int x, int y);

public:
  /**
   * Wrap horizontally?
   */
  bool wrap_h = true;

  /**
   * Wrap vertically?
   */
  bool wrap_v = false;

  /**
   * Initializes the grid to this width and height; sets everything to false
   * (dead).
   * @param ww,hh width and height
   */
  Grid(int ww, int hh) : w{ww}, h{hh}, map(ww * hh, false) {}

  /**
   * Returns the cell at this x/y.
   */
  bool at(int x, int y) const;

  /**
   * Sets a given cell
   * @param x,y coordinates
   * @param val on/off
   */
  void set(int x, int y, bool val);

  /**
   * Sets one random cell alive
   */
  void set_rand();

  /**
   * Toggles a given cell
   * @param x,y coordinates
   */
  void toggle(int x, int y);

  /**
   * Play the game of life
   */
  void tick();
};

#endif // GRID_H
