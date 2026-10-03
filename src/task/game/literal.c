#include "game_mgr.h"
#include <SDL3_image/SDL_image.h>

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);

game_task *literal_img(float x, float y, char* texture_path) {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;

	self->sprite = new_game_sprite();
	if (self->sprite == NULL) return NULL;
	self->sprite->x = x;
	self->sprite->y = y;

	size_t tp_size = strlen(texture_path) + 1;
	self->data = malloc(tp_size);
	memcpy(self->data, texture_path, tp_size);

	return self;
}

static int handle_spawn(game_task *self) {
	char *texture_path = self->data;
	self->sprite->texture = IMG_LoadTexture(self->game->renderer, texture_path);
	free(texture_path);

	if (self->sprite->texture == NULL) return -1;
	game_scale_sprite(self->sprite, 1);

	return 0;
}

static void handle_tick(game_task *self) {
	game_draw(self);
}
