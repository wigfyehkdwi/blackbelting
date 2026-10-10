#include "game_mgr.h"

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);

game_task *literal_img(float x, float y, char* texture_path) {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;

	if (game_load_sprite(self, texture_path)) return NULL;
	self->sprite->x = x;
	self->sprite->y = y;

	return self;
}

static int handle_spawn(game_task *self) {
	return 0;
}

static void handle_tick(game_task *self) {
	game_draw(self);
}
