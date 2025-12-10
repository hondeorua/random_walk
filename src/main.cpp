#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <utility>

constexpr unsigned int WIDTH = 900;
constexpr unsigned int HEIGHT = 600;

constexpr float RECT_HEIGHT = 4.0f;
constexpr float RECT_WIDTH = 4.0f;
constexpr float SPEED = 3.5f;


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

Direction RanDir();
void MoveRect(SDL_FRect &rect);
void UpdateVisited(std::vector<std::pair<int, int>> &, SDL_FRect &);

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

  SDL_Texture *ptrail1 = SDL_CreateTexture(prenderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
  if (!ptrail1) {
    std::cerr << "Trail texture failed to initialize" << SDL_GetError() << '\n';
    return 1;
  }

  SDL_Texture *ptrail2 = SDL_CreateTexture(prenderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
  if (!ptrail2) {
    std::cerr << "Trail texture failed to initialize" << SDL_GetError() << '\n';
    return 1;
  }

  SDL_Texture *ptrail3 = SDL_CreateTexture(prenderer, SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
  if (!ptrail3) {
    std::cerr << "Trail texture failed to initialize" << SDL_GetError() << '\n';
    return 1;
  }

  SDL_SetRenderTarget(prenderer, ptrail1);
  SDL_SetRenderDrawColor(prenderer, 0, 0, 0, 0); // transparent texture bg
  SDL_RenderClear(prenderer);

  SDL_SetRenderTarget(prenderer, ptrail2);
  SDL_SetRenderDrawColor(prenderer, 0, 0, 0, 00); // transparent texture bg
  SDL_RenderClear(prenderer);

  SDL_SetRenderTarget(prenderer, ptrail3);
  SDL_SetRenderDrawColor(prenderer, 0, 0, 0, 00); // transparent texture bg
  SDL_RenderClear(prenderer);

  SDL_FRect rect1{(WIDTH - RECT_WIDTH) / 2, (HEIGHT - RECT_HEIGHT) / 2, RECT_WIDTH, RECT_HEIGHT};
  Color color_rect1{255, 0, 0, 255};

  SDL_FRect rect2{(WIDTH - RECT_WIDTH) / 2, (HEIGHT - RECT_HEIGHT) / 2, RECT_WIDTH, RECT_HEIGHT};
  Color color_rect2{0, 255, 0, 255};

  SDL_FRect rect3{(WIDTH - RECT_WIDTH) / 2, (HEIGHT - RECT_HEIGHT) / 2, RECT_WIDTH, RECT_HEIGHT};
  Color color_rect3{0, 0, 255, 255};

  std::vector<std::pair<int, int>> visited;
  visited.push_back({static_cast<int>(rect1.x), static_cast<int>(rect1.y)});

  SDL_Event event;
  bool exitted = false;

  while (!exitted) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        exitted = true;
        return 0;
      }
    }

    SDL_SetRenderTarget(prenderer, ptrail1);
    if (std::find(visited.begin(), visited.end(), std::make_pair(static_cast<int>(rect1.x), static_cast<int>(rect1.y))) != visited.end()) {
      SDL_SetRenderDrawColor(prenderer, color_rect1.r, color_rect1.g, color_rect1.b, color_rect1.a);
    } else {
      SDL_SetRenderDrawColor(prenderer, color_rect1.r, color_rect1.g, color_rect1.b, 200);
    }
    SDL_RenderFillRect(prenderer, &rect1);

    SDL_SetRenderTarget(prenderer, ptrail2);
    if (std::find(visited.begin(), visited.end(), std::make_pair(static_cast<int>(rect2.x), static_cast<int>(rect2.y))) != visited.end()) {
      SDL_SetRenderDrawColor(prenderer, color_rect2.r, color_rect2.g, color_rect2.b, color_rect2.a);
    } else {
      SDL_SetRenderDrawColor(prenderer, color_rect2.r, color_rect2.g, color_rect2.b, 200);
    }
    SDL_RenderFillRect(prenderer, &rect2);

    SDL_SetRenderTarget(prenderer, ptrail3);
    if (std::find(visited.begin(), visited.end(), std::make_pair(static_cast<int>(rect3.x), static_cast<int>(rect3.y))) != visited.end()) {
      SDL_SetRenderDrawColor(prenderer, color_rect3.r, color_rect3.g, color_rect3.b, color_rect3.a);
    } else {
      SDL_SetRenderDrawColor(prenderer, color_rect3.r, color_rect3.g, color_rect3.b, 200);
    }
    SDL_RenderFillRect(prenderer, &rect3);

    SDL_SetRenderTarget(prenderer, NULL);

    SDL_SetRenderDrawColor(prenderer, 255, 255, 255, 255);
    SDL_RenderClear(prenderer);

    // SDL_SetRenderDrawColor(prenderer, color_rect.r, color_rect.g, color_rect.b, color_rect.a);
    SDL_RenderTexture(prenderer, ptrail1, NULL, NULL);
    SDL_RenderTexture(prenderer, ptrail2, NULL, NULL);
    SDL_RenderTexture(prenderer, ptrail3, NULL, NULL);

    SDL_RenderPresent(prenderer);

    MoveRect(rect1);
    UpdateVisited(visited, rect1);
    MoveRect(rect2);
    UpdateVisited(visited, rect2);
    MoveRect(rect3);
    UpdateVisited(visited, rect3);

    SDL_Delay(20);
  }
}

void MoveRect(SDL_FRect &rect) {
  switch (RanDir()) {
  case Direction::Right:
    rect.x += RECT_WIDTH;
    break;
  case Direction::Left:
    rect.x -= RECT_WIDTH;
    break;
  case Direction::Up:
    rect.y -= RECT_HEIGHT;
    break;
  case Direction::Down:
    rect.y += RECT_HEIGHT;
    break;
  }
}

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

void UpdateVisited(std::vector<std::pair<int, int>> &visited, SDL_FRect &rect) {
  visited.push_back({static_cast<int>(rect.x), static_cast<int>(rect.y)});
}
