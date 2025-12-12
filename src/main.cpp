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
#include <cmath>
#include <iostream>
#include <random>
#include <utility>

constexpr unsigned int WIDTH = 900;
constexpr unsigned int HEIGHT = 600;
constexpr float PI = 3.14159265359;

constexpr float RECT_HEIGHT = 5.0f;
constexpr float RECT_WIDTH = 5.0f;
constexpr float SPEED = 3.5f;

struct Color {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
};

class Walker {
private:
  struct Velocity {
    float vx = 0;
    float vy = 0;
  };
  SDL_FRect rect;
  Color color;
  Velocity v;
  static inline int num_dir;

  SDL_Texture *ptrail;
  SDL_Renderer *prenderer;
  // static std::vector<std::pair<int, int>> visited;
  // visited.push_back({static_cast<int>(rect1.x), static_cast<int>(rect1.y)});

  inline static std::mt19937 rng{
      static_cast<unsigned>(
          std::chrono::high_resolution_clock::now().time_since_epoch().count())};

  static float RanRad() {
    static std::uniform_int_distribution<int> dist(1, num_dir);
    return static_cast<float>(dist(rng)) / num_dir * 2 * PI;
  }

  void UpdateVelocity() {
    auto rad = RanRad();
    v.vx = SPEED * std::cos(rad);
    v.vy = SPEED * std::sin(rad);
  }

public:
  Walker(const Color &color)
      : rect{(WIDTH - RECT_WIDTH) / 2, (HEIGHT - RECT_HEIGHT) / 2, RECT_WIDTH, RECT_HEIGHT}
      , color(color) {}

  Walker(int num_dir, const Color &color, SDL_Renderer *renderer, SDL_Texture *trail)
      : prenderer(renderer)
      , ptrail(trail)
      , rect{(WIDTH - RECT_WIDTH) / 2, (HEIGHT - RECT_HEIGHT) / 2, RECT_WIDTH, RECT_HEIGHT}
      , color(color) {
    Walker::num_dir = num_dir;
  }

  const SDL_FRect &GetRect() const { return rect; }
  const Color &GetColor() const { return color; }
  const Velocity &GetVelocity() const { return v; }

  void MoveWalker() {
    float prev_x = rect.x;
    float prev_y = rect.y;

    rect.x += v.vx;
    rect.y += v.vy;

    SDL_SetRenderTarget(prenderer, ptrail);
    SDL_SetRenderDrawColor(prenderer, color.r, color.g, color.b, color.a);
    SDL_RenderLine(prenderer, prev_x, prev_y, rect.x, rect.y);

    UpdateVelocity();
  }
};

// void UpdateVisited(std::vector<std::pair<int, int>> &, SDL_FRect &);

int main(int argc, char *argv[]) {
  std::cout << "Program's starting...\n";

  const int num_dir = 4;

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

  Walker w1(num_dir, (Color){255, 0, 0, 255}, prenderer, ptrail);

  SDL_Event event;
  bool exitted = false;

  while (!exitted) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        exitted = true;
        return 0;
      }
    }

    SDL_SetRenderTarget(prenderer, NULL);
    SDL_SetRenderDrawColor(prenderer, 255, 255, 255, 255);
    SDL_RenderClear(prenderer);

    // SDL_SetRenderDrawColor(prenderer, color_rect.r, color_rect.g, color_rect.b, color_rect.a);
    SDL_RenderTexture(prenderer, ptrail, NULL, NULL);

    SDL_RenderPresent(prenderer);

    w1.MoveWalker();
    // UpdateVisited(visited, rect1);

    SDL_Delay(20);
  }
}

// void UpdateVisited(std::vector<std::pair<int, int>> &visited, SDL_FRect &rect) {
//   visited.push_back({static_cast<int>(rect.x), static_cast<int>(rect.y)});
// }
