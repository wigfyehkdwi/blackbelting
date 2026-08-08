#include "weapon.h"
#include "game_mgr.h"
#include <SDL3_image/SDL_image.h>

static int handle_spawn(game_task *self);
static float get_x_offset(game_task *self);
static void handle_tick(game_task *self);
static void handle_event(game_task *self);

weapon_type weapon_sword = {
	.type = MELEE,
	.sprite_path = "res/game/sword.png",
	.scale = 3
};

weapon_type weapon_gun = {
	.type = RANGED,
	.sprite_path = "res/game/gun.png",
	.scale = 2.2
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
	data->types[0] = type;
	self->data = data;

	return self;
}

int apply_weapon(game_task *self, int idx) {
	weapon_data *data = self->data;
	data->selection = idx;
	if (idx < 0) return 0;
	weapon_type *type = data->types[idx];

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
	if (apply_weapon(self, data->selection)) return -1;
	return 0;
}

static float get_x_offset(game_task *self) {
	weapon_data *data = self->data;
	float owner_w = data->owner->sprite->w;

	if (self->sprite->flip == SDL_FLIP_HORIZONTAL) return -owner_w + 16;
	return owner_w - 4;
}

static void handle_tick(game_task *self) {
	weapon_data *data = self->data;
	if (data->selection < 0) return;

	game_sprite *owner_spr = data->owner->sprite;
	float old_x = self->sprite->x - get_x_offset(self);
	if (owner_spr->x < old_x) self->sprite->flip = SDL_FLIP_HORIZONTAL;
	else if (owner_spr->x > old_x) self->sprite->flip = SDL_FLIP_NONE;

	self->sprite->x = owner_spr->x + get_x_offset(self);
	self->sprite->y = owner_spr->y + 3;
//printf("owner_spr->x = %f, old_x = %f, self->sprite->x = %f")
	game_draw(self);

}

static void handle_event(game_task *self) {
	game_services *svc = self->game->manager->data;
	
}
