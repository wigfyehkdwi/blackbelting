#include "weapon.h"
#include <SDL3_image/SDL_image.h>

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);
static void handle_event(game_task *self);

weapon_type weapon_sword = {
	.type = MELEE,
	.sprite_path = "res/game/sword.png",
	.scale = 3
};

game_task *weapon(game_task *owner, weapon_type *type) {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;
	self->on_event = handle_event;

	weapon_data *data = calloc(sizeof(weapon_data), 1);
	if (data == NULL) return NULL;
	data->owner = owner;
	data->type = type;
	self->data = data;

	return self;
}

int apply_weapon(game_task *self, weapon_type *type) {
	weapon_data *data = self->data;
	data->type = type;

	if (self->sprite == NULL) self->sprite = new_game_sprite();
	if (self->sprite == NULL) return -1;
	self->sprite->texture = IMG_LoadTexture(self->game->renderer, type->sprite_path);
	if (self->sprite->texture == NULL) return -1;
	SDL_SetTextureScaleMode(self->sprite->texture, SDL_SCALEMODE_PIXELART);
	game_scale_sprite(self->sprite, type->scale);
	return 0;
}

static int handle_spawn(game_task *self) {
	weapon_data *data = self->data;
	if (apply_weapon(self, data->type)) return -1;
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
