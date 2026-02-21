#include "play_button.h"
#include "../game/game_mgr.h"
#include <SDL3_image/SDL_image.h>

static void handle_tick(game_task *self);
static void handle_event(game_task *self);

int play_button(game_task *self) {
	self->on_tick = handle_tick;
	self->on_event = handle_event;

	self->sprite = game_new_sprite();
	if (self->sprite == NULL) return -1;

	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/main_menu/play.png");
	if (self->sprite->texture == NULL) return -1;

	self->sprite->x = 300;
	self->sprite->y = 300;
	self->sprite->ui = true;
	return 0;
}

static void handle_tick(game_task *self) {
	game_draw(self);
}

static void handle_event(game_task *self) {
	if (game_is_clicked(self) == 2) {
		game_killall(self);
		game_spawn(self, game_mgr);
	}
}
