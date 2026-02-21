#include "main_menu.h"

#include "title.h"
#include "play_button.h"

static void handle_tick(game_task *self);
static void handle_event(game_task *self);

int main_menu(game_task *self) {
	self->on_tick = handle_tick;
	self->on_event = handle_event;

	/* become the game manager */
	self->game->mgr = self;

	/* spawn sub-tasks */
	if (game_spawn(self, title)) return 1;
	if (game_spawn(self, play_button)) return 1;

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
