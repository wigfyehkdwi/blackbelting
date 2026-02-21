#include "title.h"

#include <SDL3_image/SDL_image.h>
static int load_resources(game_task *self);
static void handle_tick(game_task *self);

game_task *title() {
	game_task *task = game_new_task();
	if (task == NULL) return NULL;

	task->on_spawn = load_resources;
	task->on_tick = handle_tick;
	return task;
}

static int load_resources(game_task *self) {
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
