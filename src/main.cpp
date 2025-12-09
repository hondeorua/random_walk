#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <chrono>
#include <iostream>
#include <random>

constexpr unsigned int WIDTH = 900;
constexpr unsigned int HEIGHT = 600;

constexpr float RECT_HEIGHT = 7.0f;
constexpr float RECT_WIDTH = 7.0f;

struct Color {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
};

enum class Direction {
  Right,
  Left,
  Up,
  Down
};

Direction RanDir() {
  static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  static std::uniform_int_distribution<int> dist(1, 4);
  switch (dist(rng)) {
  case 1:
    return Direction::Right;
  case 2:
    return Direction::Left;
  case 3:
    return Direction::Up;
  case 4:
    return Direction::Down;
  }
  std::cerr << "Impossible case: dist should only generate 1 -> 4\n";
  return Direction::Right;
}

int main(int argc, char *argv[]) {
  std::cout << "Program's starting...\n";

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL initialization failed: " << SDL_GetError() << '\n';
    return 1;
  }

  SDL_Window *pwindow = SDL_CreateWindow("Walking...", WIDTH, HEIGHT, SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_MOUSE_FOCUS);

  if (!pwindow) {
    std::cerr << "Window failed to create: " << SDL_GetError() << '\n';
    return 1;
  }

  SDL_Renderer *prenderer = SDL_CreateRenderer(pwindow, NULL);
  if (!prenderer) {
    std::cerr << "Render failed to initialize" << SDL_GetError() << '\n';
    return 1;
  }

  SDL_Texture *ptrail = SDL_CreateTexture(prenderer, SDL_PIXELFORMAT_RGBA8888,
                                          SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
  if (!ptrail) {
    std::cerr << "Trail texture failed to initialize" << SDL_GetError() << '\n';
    return 1;
  }

  SDL_SetRenderTarget(prenderer, ptrail);
  SDL_SetRenderDrawColor(prenderer, 0, 0, 0, 0); // transparent texture bg
  SDL_RenderClear(prenderer);

  SDL_FRect rect{(WIDTH - RECT_WIDTH) / 2, (HEIGHT - RECT_HEIGHT) / 2, RECT_WIDTH, RECT_HEIGHT};
  Color color_rect{255, 0, 0, 255};

  SDL_Event event;
  bool exitted = false;

  while (!exitted) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        exitted = true;
        return 0;
      }
    }

    SDL_SetRenderTarget(prenderer, ptrail);
    SDL_SetRenderDrawColor(prenderer, color_rect.r, color_rect.g, color_rect.b, color_rect.a);
    SDL_RenderFillRect(prenderer, &rect);

    SDL_SetRenderTarget(prenderer, NULL);

    SDL_SetRenderDrawColor(prenderer, 28, 28, 28, 255);
    SDL_RenderClear(prenderer);

    // SDL_SetRenderDrawColor(prenderer, color_rect.r, color_rect.g, color_rect.b, color_rect.a);
    SDL_RenderTexture(prenderer, ptrail, NULL, NULL);

    SDL_RenderPresent(prenderer);

    switch (RanDir()) {
    case Direction::Right:
      rect.x += RECT_WIDTH * 1.2;
      break;
    case Direction::Left:
      rect.x -= RECT_WIDTH * 1.2;
      break;
    case Direction::Up:
      rect.y -= RECT_HEIGHT * 1.2;
      break;
    case Direction::Down:
      rect.y += RECT_HEIGHT * 1.2;
      break;
    }

    SDL_Delay(20);
  }
}
