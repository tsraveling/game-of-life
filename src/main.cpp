#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <cstdio>
#include <iostream>
#include <string>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "drawing/Drawing.h"
#include "entities/Grid.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "logging/Logger.h"

using namespace std;

constexpr int kWindowWidth{800};
constexpr int kWindowHeight{600};

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
  auto keyboard = new Keyboard();

  logger->log("Up and running ...");

  auto grid = new Grid(64, 64);

  int ticks_per_second = 4;
  bool paused = true;

  float cell_size = 32;
  const float MIN_CELL_SIZE = 8;
  const float MAX_CELL_SIZE = 48;

  const int SEED_AMOUNT = 50;

  const float CAM_SPEED = 320.0;

  float cam_x = 0;
  float cam_y = 0;
  float last_ms = 0;

  Uint64 moment_last = SDL_GetPerformanceCounter();
  Uint64 frequency = SDL_GetPerformanceFrequency();
  double delta = 0;
  double delta_counter = 0;

  // Main loop
  while (!quit) {

    // Get delta
    Uint64 moment_now = SDL_GetPerformanceCounter();
    delta = (double)(moment_now - moment_last) / frequency;
    moment_last = moment_now;

    // Handle events on queue
    while (SDL_PollEvent(&e) != 0) {

      // Pass the event to the keyboard handler
      keyboard->feed(e);

      if (e.type == SDL_EVENT_KEY_DOWN) {
        switch (e.key.key) {
        case SDLK_MINUS:
          cell_size = cell_size > MIN_CELL_SIZE ? cell_size - 2 : MIN_CELL_SIZE;
          break;
        case SDLK_EQUALS:
          cell_size = cell_size < MAX_CELL_SIZE ? cell_size + 2 : MAX_CELL_SIZE;
          break;
        case SDLK_LEFTBRACKET:
          ticks_per_second = ticks_per_second > 1 ? ticks_per_second - 1 : 1;
          break;
        case SDLK_RIGHTBRACKET:
          ticks_per_second = ticks_per_second < 60 ? ticks_per_second + 1 : 60;
          break;
        case SDLK_N:
          for (int i = 0; i < SEED_AMOUNT; i++) {
            grid->set_rand();
          }
          break;
        case SDLK_SPACE:
          paused = !paused;
          break;
        }
      }

      // User requests quit
      if (e.type == SDL_EVENT_QUIT) {
        quit = true;
      }
    }

    // Do logic
    if (!paused) {
      delta_counter += delta;
      if (delta_counter >= 1.0 / ticks_per_second) {
        delta_counter = 0;
        grid->tick();
      }
    }

    // Handle camera
    auto move = keyboard->get_move();
    cam_x += move.x * delta * CAM_SPEED;
    cam_y += move.y * delta * CAM_SPEED;

    // Clear screen
    if (paused) {
      SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    } else {

      SDL_SetRenderDrawColor(renderer, 64, 128, 64, 255);
    }
    SDL_RenderClear(renderer);

    // Handle input
    mouse->tick();

    // Draw the grid
    int w = 64;
    int h = 64;
    for (int x = 0; x < w; x++) {
      for (int y = 0; y < h; y++) {
        if (grid->at(x, y)) {
          draw->set_color(Colors::RED);
        } else {
          draw->set_color(Colors::BLACK);
        }
        draw->rect((x * cell_size) + 1 - cam_x, (y * cell_size) + 1 - cam_y,
                   cell_size - 2, cell_size - 2);
      }
    }

    // Get the position
    int mox = (mouse->mx() + cam_x) / cell_size;
    int moy = (mouse->my() + cam_y) / cell_size;
    draw->set_color(Colors::WHITE);
    draw->rect(mox * cell_size - cam_x, moy * cell_size - cam_y, cell_size, 1);
    draw->rect(mox * cell_size - cam_x, (moy + 1) * cell_size - cam_y,
               cell_size, 1);
    draw->rect(mox * cell_size - cam_x, moy * cell_size - cam_y, 1, cell_size);
    draw->rect((mox + 1) * cell_size - cam_x, moy * cell_size - cam_y, 1,
               cell_size);

    if (mouse->clicked() && mox <= 64 && moy <= 64) {
      grid->toggle(mox, moy);
    }

    // Handle pause / unpause
    if (mouse->clicked(RIGHT)) {
      paused = !paused;
    }

    // GUI
    draw->dbg_print(5, 5, "SPEED: %d ([] to change)", ticks_per_second);
    draw->dbg_print(5, 25, "CAM: %.1f, %.1f (WASD to scroll)", cam_x, cam_y);
    draw->dbg_print(5, 45, "CELL_SIZE: %.1f (-+ to zoom)", cell_size);
    if (paused)
      draw->dbg_print(300, 5, "SPACE TO UNPAUSE");

    // Update screen
    SDL_RenderPresent(renderer);
  }

  // SDL Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
