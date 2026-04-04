#include "axolotl.h"
#include "game_mgr.h"
#include <SDL3_image/SDL_image.h>

typedef struct {
	/* key states */
	bool up;
	bool down;
	bool left;
	bool right;
} axolotl_data;

static void handle_tick(game_task *self);

int axolotl(game_task *self) {
	self->on_tick = handle_tick;
	self->sprite = calloc(sizeof(game_sprite), 1);
	self->sprite->x = 69;
	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/game/axolotl.jpg");
	if (self->sprite->texture == NULL) return -1;

	return 0;
}

static void handle_tick(game_task *self) {
	game_draw(self);
}
