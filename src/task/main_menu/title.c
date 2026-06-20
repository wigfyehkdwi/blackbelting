#include "title.h"
#include <SDL3_image/SDL_image.h>

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);

game_task *title() {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;
	return self;
}

static int handle_spawn(game_task *self) {
	self->sprite = new_game_sprite();
	if (self->sprite == NULL) return -1;

	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/main_menu/title.png");
	if (self->sprite->texture == NULL) return -1;
	game_scale_sprite(self->sprite, 1);

	self->sprite->x = 0;
	self->sprite->y = 0;
	self->sprite->ui = true;
	return 0;
}

static void handle_tick(game_task *self) {
	game_draw(self);
}
