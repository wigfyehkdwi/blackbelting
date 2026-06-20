#include "title.h"
#include <SDL3_image/SDL_image.h>

static void handle_tick(game_task *self);

game_task *title() {
	game_task *self = new_game_task();
	self->on_tick = handle_tick;

	self->sprite = new_game_sprite();
	if (self->sprite == NULL) return NULL;

	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/main_menu/title.png");
	if (self->sprite->texture == NULL) return NULL;
	game_scale_sprite(self->sprite, 1);

	self->sprite->x = 0;
	self->sprite->y = 0;
	self->sprite->ui = true;
	return self;
}

static void handle_tick(game_task *self) {
	game_draw(self);
}
