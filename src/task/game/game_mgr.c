#include "game_mgr.h"
#include "player.h"

static void handle_tick(game_task *self);
static void handle_event(game_task *self);

int game_mgr(game_task *self) {
	self->on_tick = handle_tick;
	self->on_event = handle_event;

	game_services *svc = calloc(sizeof(game_services), 1);
	self->data = svc;
	/* hardcode keys for now */
	svc->keys.up = SDLK_UP;
	svc->keys.down = SDLK_DOWN;
	svc->keys.left = SDLK_LEFT;
	svc->keys.right = SDLK_RIGHT;


	/* become the game manager */
	self->game->manager = self;

	/* spawn sub-tasks */
	if (game_spawn(self, player)) return 1;

	return 0;
}

static void handle_tick(game_task *self) {
	SDL_SetRenderDrawColor(self->game->renderer, 0, 12, 214, 255);
	SDL_RenderClear(self->game->renderer);
}

static void handle_event(game_task *self) {
	if (self->game->event.type == SDL_EVENT_QUIT) {
		self->game->exit = true;
	}
}
