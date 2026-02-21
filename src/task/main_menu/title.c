#include "title.h"
#include <SDL3_image/SDL_image.h>

static void handle_tick(game_task *self);

int title(game_task *self) {
	self->on_tick = handle_tick;

	self->sprite = game_new_sprite();
	if (self->sprite == NULL) return -1;

	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/main_menu/title.png");
	if (self->sprite->texture == NULL) return -1;

	self->sprite->x = 0;
	self->sprite->y = 0;
	self->sprite->ui = true;
	return 0;
}

static void handle_tick(game_task *self) {
	game_draw(self);
}
