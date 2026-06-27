#include "../../core/game.h"
#include <SDL3_image/SDL_image.h>

enum attack_type {
	MELEE
};

typedef struct {
	game_task *owner;
	enum attack_type type;
} weapon_data;

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);
static void handle_event(game_task *self);

game_task *weapon(game_task *owner) {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;
	self->on_event = handle_event;

	weapon_data *data = calloc(sizeof(weapon_data), 1);
	if (data == NULL) return NULL;
	data->owner = owner;
	self->data = data;

	return self;
}

static int handle_spawn(game_task *self) {
	self->sprite = calloc(sizeof(game_sprite), 1);
	if (self->sprite == NULL) return -1;
	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/game/sword.png");
	if (self->sprite->texture == NULL) return -1;
	SDL_SetTextureScaleMode(self->sprite->texture, SDL_SCALEMODE_PIXELART);
	game_scale_sprite(self->sprite, 3);
	return 0;
}

static void handle_tick(game_task *self) {
	weapon_data *data = self->data;
	game_sprite *owner_spr = data->owner->sprite;
	self->sprite->x = owner_spr->x + owner_spr->w - 4;
	self->sprite->y = owner_spr->y + 3;
	game_draw(self);
}

static void handle_event(game_task *self) {
}
