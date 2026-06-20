#include "play_button.h"
#include "../game/game_mgr.h"
#include <SDL3_image/SDL_image.h>

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);
static void handle_event(game_task *self);

game_task *play_button() {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;
	self->on_event = handle_event;
	return self;
}

static int handle_spawn(game_task *self) {
	self->sprite = new_game_sprite();
	if (self->sprite == NULL) return -1;

	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/main_menu/play.png");
	if (self->sprite->texture == NULL) return -1;
	game_scale_sprite(self->sprite, 1);

	self->sprite->x = 300;
	self->sprite->y = 300;
	self->sprite->ui = true;
	return 0;
}

static void handle_tick(game_task *self) {
	game_draw(self);
}

static void handle_event(game_task *self) {
	game_task *root = &self->game->tasks;
	if (game_is_clicked(self) == 2) {
		game_killall(self);
		game_spawn(root, game_mgr());
	}
}
