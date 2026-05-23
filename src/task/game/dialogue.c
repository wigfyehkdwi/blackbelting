#include "dialogue.h"
#include "game_mgr.h"
#include <SDL3_image/SDL_image.h>

typedef struct {
	/* key states */
	bool up;
	bool down;
	bool left;
	bool right;
} dialogue_data;

static void handle_tick(game_task *self);

int dialogue(game_task *self) {
	self->on_tick = handle_tick;

        game_task *mgr = self->game->manager;
        game_services *svc = mgr->data;
        svc->dialogue = self;

	return 0;
}

static void handle_tick(game_task *self) {
	SDL_FRect rect = {.x = 0, .y = 480 - 100, .w = 640, .h = 100}; /* TODO: Unhardcode */
	SDL_SetRenderDrawColor(self->game->renderer, 0, 0, 17, 255);
	SDL_RenderFillRect(self->game->renderer, &rect);
}
