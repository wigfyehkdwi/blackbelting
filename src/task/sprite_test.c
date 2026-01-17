#include "../core/game.h"
#include "quit_handler.h"

#include <SDL3_image/SDL_image.h>
static int load_resources(game_task *self);
static void handle_tick(game_task *self);

game_task *sprite_test() {
	game_task *task = game_new_task();
	if (task == NULL) return NULL;

	task->on_spawn = load_resources;
	task->on_tick = handle_tick;
	return task;
}

static int load_resources(game_task *self) {
	self->sprite = game_new_sprite();
	if (self->sprite == NULL) return -1;

	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/test.png");
	if (self->sprite->texture == NULL) return -1;

	return 0;
}

static void handle_tick(game_task *self) {
	self->sprite->x = 6;
	self->sprite->y = 9;
	game_draw(self->sprite, self->game->renderer);
}
