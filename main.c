#include "game.h"

int main(int argc, char *argv[]) {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Test window", 640, 480, 0);

	if (window == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
		return 1;
	}

	game_state game;
	game_init(&game);

	while (1) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			game_fire(&game, &event);
		}
		game_tick(&game);
	}
}
