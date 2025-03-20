#include "Grid.h"

// Return the cell at given coordinate
bool Grid::at(int x, int y) const { return map[y * w + x]; }

// Set a given cell
void Grid::set(int x, int y, bool val) { map[y * w + x] = val; }

// Toggle a given cell
void Grid::toggle(int x, int y) {
  int i = y * w + x;
  map[i] = !map[i];
}

// Check an offset from a given index, and handle wrapping
bool Grid::check(int at_x, int at_y, int x, int y) {
  int tar_x = at_x - x;
  int tar_y = at_y - y;
  if (tar_x < 0) {
    if (wrap_h) {
      tar_x += w;
    } else {
      return false;
    }
  }
  if (tar_x >= w) {
    if (wrap_h) {
      tar_x -= w;
    } else {
      return false;
    }
  }
  if (tar_y < 0) {
    if (wrap_v) {
      tar_y -= h;
    } else {
      return false;
    }
  }
  if (tar_y >= h) {
    if (wrap_v) {
      tar_y -= h;
    } else {
      return false;
    }
  }

  return map[tar_y * w + tar_x];
}

// Iterate through the immediate neighbors of a given cell, wrapping
// using the check method if desired. Return the number of live
// neighbors.
int Grid::count_neighbors(int x, int y) {
  int c = 0;
  for (int xo = -1; xo <= 1; xo++) {
    for (int yo = -1; yo <= 1; yo++) {
      if (xo != 0 || yo != 0) {
        c += check(x, y, xo, yo) ? 1 : 0;
      }
    }
  }
  return c;
}

// Play the Game of Life
// 1. A live cell with less than 2 or more than 3 neighbors dies
// 2. A live cell with either 2 or 3 neighbors survives
// 3. A dead cell with exactly 3 live neighbors becomes alive
void Grid::tick() {
  std::vector<int> to_toggle;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int index = y * w + x;
      bool alive = at(x, y);
      int neighbors = count_neighbors(x, y);
      if (alive) {

        // Kill any live cells with less than 2 or more than 3 live neighbors
        if (neighbors < 2 || neighbors > 3) {
          to_toggle.push_back(index);
        }
      } else {

        // Spawn any dead cells with exactly 3 neighbors
        if (neighbors == 3) {
          to_toggle.push_back(index);
        }
      }
    }
  }

  // Commit the changes by toggling each index we captured above
  for (int i : to_toggle) {
    map[i] = !map[i];
  }
}
