#include "enemy.h"
#include "game_mgr.h"
#include <SDL3_image/SDL_image.h>

enum enemy_phase {
	IDLE,
	DIALOGUE,
	FIGHT,
	DEAD
};

typedef struct {
	int phase;
} enemy_data;

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);

game_task *enemy() {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;
	return self;
}

static int handle_spawn(game_task *self) {
	self->sprite = calloc(sizeof(game_sprite), 1);
	if (self->sprite == NULL) return -1;
	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/game/enemy.png");
	if (self->sprite->texture == NULL) return -1;
	game_scale_sprite(self->sprite, 1);

	return 0;
}

static void handle_tick(game_task *self) {
	game_draw(self);
}
