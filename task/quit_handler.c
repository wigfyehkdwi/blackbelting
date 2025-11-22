#include "../core/game.h"
#include "quit_handler.h"

static void handle_tick(game_task *self);

game_task *quit_handler() {
	game_task *task = game_new_task();
	task->on_event = handle_tick;
	return task;
}

static void handle_tick(game_task *self) {
	if (self->event->type == SDL_EVENT_QUIT) {
		self->game->aborted = true;
	}
}
