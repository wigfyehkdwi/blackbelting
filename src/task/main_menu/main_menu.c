#include "main_menu.h"

#include "title.h"
#include "play_button.h"

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);
static void handle_event(game_task *self);

game_task *main_menu() {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;
	self->on_event = handle_event;
	return self;
}

static int handle_spawn(game_task *self) {
	/* become the game manager */
	self->game->manager = self;

	/* spawn sub-tasks */
	if (game_spawn(self, title())) return -1;
	if (game_spawn(self, play_button())) return -1;
	return 0;
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
