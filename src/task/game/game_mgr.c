#include "game_mgr.h"

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);
static void handle_event(game_task *self);

game_task *game_mgr() {
	game_task *task = game_new_task();
	task->on_spawn = handle_spawn;
	task->on_tick = handle_tick;
	task->on_event = handle_event;
	return task;
}

static int handle_spawn(game_task *self) {
	/* become the game manager */
	self->game->mgr = self;

	/* spawn sub-tasks */
	/*if (game_spawn(self->game, title())) return 1;
	if (game_spawn(self->game, play_button())) return 1;*/

	return 0;
}

static void handle_tick(game_task *self) {
	SDL_SetRenderDrawColor(self->game->renderer, 0, 255, 255, 255);
	SDL_RenderClear(self->game->renderer);
}

static void handle_event(game_task *self) {
	if (self->game->event.type == SDL_EVENT_QUIT) {
		self->game->exit = true;
	}
}
