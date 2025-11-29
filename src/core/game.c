#include "game.h"

void game_spawn(game_state *game, game_task *task) {
	game->tasks.prev->next = task;
	task->prev = game->tasks.prev;

	game->tasks.prev = task;
	task->next = &game->tasks;

	task->game = game;
}

void game_kill(game_task *task) {
	task->game->tasks.prev = task->prev;
	task->game->tasks.next = task->next;

	task->free(task);
}

void game_init(game_state *game) {
	memset(game, 0, sizeof(*game));
	game->tasks.prev = &game->tasks;
	game->tasks.next = &game->tasks;
}

void game_nop(game_task *self) {
}

static void game_default_free(game_task *self);
static void game_default_free(game_task *self) {
	free(self);
}

game_task *game_new_task() {
	game_task *task = calloc(sizeof(game_task), 1);
	task->on_event = game_nop;
	task->on_tick = game_nop;
	task->free = game_default_free;
	return task;
}

int game_switch_event(game_task *task, uint32_t event_type, void (*handler)(game_task *)) {
	int flag = task->game->event.type == event_type;

	if (flag) handler(task);
	return flag;
}

void game_event(game_state *game) {
	for (game_task *task = game->tasks.next; task != &game->tasks; task = task->next) {
		task->on_event(task);
	}
}

void game_tick(game_state *game) {
	SDL_RenderClear(game->renderer);
	for (game_task *task = game->tasks.next; task != &game->tasks; task = task->next) {
		task->on_tick(task);
	}
	SDL_RenderPresent(game->renderer);
}
