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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Handle input
    mouse->tick();

    // Draw the grid
    draw->set_color(Colors::GRAY);
    int w = 64;
    int h = 64;
    for (int x = 0; x < w; x++) {
      draw->rect(x * kCellSize, 0, 1, h * kCellSize);
      for (int y = 0; y < h; y++) {
        draw->rect(0, y * kCellSize, w * kCellSize, 1);
      }
    }

    // Get the position
    int mox = mouse->mx() / kCellSize;
    int moy = mouse->my() / kCellSize;
    if (mouse->down()) {
      draw->set_color(0, 255, 0);
    } else {
      draw->set_color(Colors::RED);
    }
    draw->rect(mox * kCellSize, moy * kCellSize, kCellSize, kCellSize);

    // Update screen
    SDL_RenderPresent(renderer);
  }

  // SDL Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
