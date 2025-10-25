#include <SDL3/SDL.h>

int main(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Test window", 420, 69, 0);

  if (window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
    return 1;
  }

  SDL_Delay(9001);
}
