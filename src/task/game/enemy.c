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

static void handle_tick(game_task *self);

game_task *enemy() {
	game_task *self = new_game_task();
	self->on_tick = handle_tick;
	self->sprite = calloc(sizeof(game_sprite), 1);
	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/game/enemy.png");
	if (self->sprite->texture == NULL) return NULL;
	game_scale_sprite(self->sprite, 1);

	return self;
}

static void handle_tick(game_task *self) {
	game_draw(self);
}
