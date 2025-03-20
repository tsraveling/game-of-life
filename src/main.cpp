#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdio>
#include <iostream>

#include "drawing/Drawing.h"
#include "input/Mouse.h"
#include "logging/Logger.h"

using namespace std;

constexpr int kWindowWidth{800};
constexpr int kWindowHeight{600};
constexpr float kCellSize{32};

int main(int argc, char *argv[]) {

  // std::cout.setf(std::ios::unitbuf);

  cout << "Begin!" << endl;

  // Initialize SDL
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return 1;
  }

  // Create window
  SDL_Window *window =
      SDL_CreateWindow("SDL3 Window", kWindowWidth, kWindowHeight, 0);
  if (!window) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return 1;
  }

  // Create renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // Enable vsync
  SDL_SetRenderVSync(renderer, 1);

  // Main loop flag
  bool quit = false;

  // Event handler
  SDL_Event e;

  // Set up the drawer
  auto draw = new Draw(renderer);
  auto logger = new Logger();
  auto mouse = new Mouse();

  logger->log("Up and running ...");

  bool grid[64][64];

  // Main loop
  while (!quit) {

    // Handle events on queue
    while (SDL_PollEvent(&e) != 0) {
      // User requests quit
      if (e.type == SDL_EVENT_QUIT) {
        quit = true;
      }
    }

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);

    // Handle input
    mouse->tick();

    // Draw the grid
    draw->set_color(Colors::GRAY);
    int w = 64;
    int h = 64;
    for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
        if (grid[x][y]) {
          draw->set_color(Colors::RED);
        } else {
          draw->set_color(Colors::BLACK);
        }
        draw->rect((x * kCellSize) + 1, (y * kCellSize) + 1, kCellSize - 2,
                   kCellSize - 2);
      }
    }

    // Get the position
    int mox = mouse->mx() / kCellSize;
    int moy = mouse->my() / kCellSize;
    draw->set_color(Colors::WHITE);
    draw->rect(mox * kCellSize, moy * kCellSize, kCellSize, 1);
    draw->rect(mox * kCellSize, (moy + 1) * kCellSize, kCellSize, 1);
    draw->rect(mox * kCellSize, moy * kCellSize, 1, kCellSize);
    draw->rect((mox + 1) * kCellSize, moy * kCellSize, 1, kCellSize);

    if (mouse->clicked() && mox <= 64 && moy <= 64) {
      grid[mox][moy] = !grid[mox][moy];
    }

    // Update screen
    SDL_RenderPresent(renderer);
  }

  // SDL Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
