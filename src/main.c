#include "core/game.h"
#include "task/quit_handler.h"
#include "task/sprite_test.h"

int main(int argc, char *argv[]) {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Test window", 640, 480, SDL_WINDOW_OPENGL);

	if (window == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
		return 1;
	}

	game_state game;
	game_init(&game);
	game.window = window;

	game.renderer = SDL_CreateRenderer(window, NULL);

	if (game.renderer == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to get renderer: %s", SDL_GetError());
	}

	/* test code */
	game_spawn(&game, quit_handler());
	game_spawn(&game, sprite_test());

	while (!game.aborted) {
		while (SDL_PollEvent(&game.event)) {
			game_event(&game);
		}
		game_tick(&game);
	}
}
