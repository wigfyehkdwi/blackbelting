#include "game_mgr.h"
#include "player.h"
#include "enemy.h"
#include "axolotl.h"

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);
static void handle_event(game_task *self);

game_task *game_mgr() {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;
	self->on_event = handle_event;

	game_services *svc = calloc(sizeof(game_services), 1);
	if (svc == NULL) return NULL;
	self->data = svc;
	/* hardcode keys for now */
	svc->keys.up = SDLK_UP;
	svc->keys.down = SDLK_DOWN;
	svc->keys.left = SDLK_LEFT;
	svc->keys.right = SDLK_RIGHT;
	svc->keys.interact = SDLK_SPACE;
	svc->keys.proceed = SDLK_SPACE; /* SDLK_RETURN; */
	return self;
}

static int handle_spawn(game_task *self) {
	/* become the game manager */
	self->game->manager = self;

	/* spawn sub-tasks */
	if (game_spawn(self, player())) goto fail;
	if (game_spawn(self, enemy())) goto fail;
	if (game_spawn(self, axolotl())) goto fail;
	return 0;

fail:
	self->game->exit = true;
	return -1;
}

static void handle_tick(game_task *self) {
	SDL_SetRenderDrawColor(self->game->renderer, 12, 214, 255, 255);
	SDL_RenderClear(self->game->renderer);
}

static void handle_event(game_task *self) {
	if (self->game->event.type == SDL_EVENT_QUIT) {
		self->game->exit = true;
	}
}
