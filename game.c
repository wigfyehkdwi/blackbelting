#include "game.h"

int game_add_task(game_state *game, game_task *task) {
	game->tasks.prev->next = task;
	task->prev = game->tasks.prev;

	game->tasks.prev = task;
	task->next = &game->tasks;
}

int game_init(game_state *game) {
	/* memset(game, 0, sizeof(*game)); */
	game->tasks.prev = &game->tasks;
	game->tasks.next = &game->tasks;
}

void game_nop(game_task *self) {
}

int game_init_task(game_task *task) {
	memset(task, 0, sizeof(*task));
	task->on_event = game_nop;
	task->on_tick = game_nop;
	task->free = game_nop;
}

int game_switch_event(game_task *task, uint32_t event_type, void (*handler)(game_task *)) {
	int flag = task->event->type == event_type;

	if (flag) handler(task);
	return flag;
}

void game_free(game_task *task) {
	task->free(task);
}

void game_fire(game_state *game, SDL_Event *event) {
	for (game_task *task = game->tasks.next; task != &game->tasks; task = task->next) {
		task->event = event;
		task->on_event(task);
	}
}

void game_tick(game_state *game) {
	for (game_task *task = game->tasks.next; task != &game->tasks; task = task->next) {
		task->on_tick(task);
	}
}
