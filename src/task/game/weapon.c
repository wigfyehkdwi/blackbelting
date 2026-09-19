#include "weapon.h"
#include "game_mgr.h"
#include "player.h"
#include "adapter.h"
#include <SDL3_image/SDL_image.h>

static int handle_spawn(game_task *self);
static float get_x_offset(game_task *self);
static void handle_tick(game_task *self);
static void do_attack(game_task *self);
static void handle_event(game_task *self);

weapon_type weapon_sword = {
	.type = MELEE,
	.sprite_path = "res/game/sword.png",
	.scale = 3,
	.attack_damage = 3
};

weapon_type weapon_gun = {
	.type = RANGED,
	.sprite_path = "res/game/gun.png",
	.scale = 2.2,
	.attack_damage = 3
};

game_task *weapon(weapon_type *type) {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->magic = WEAPON_MAGIC;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;
	self->on_event = handle_event;

	weapon_data *data = calloc(sizeof(weapon_data), 1);
	if (data == NULL) return NULL;
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
	float owner_w = self->parent->sprite->w;
	if (self->sprite->flip == SDL_FLIP_HORIZONTAL) return -owner_w + 16;
	return owner_w - 4;
}

static void handle_tick(game_task *self) {
	weapon_data *data = self->data;
	if (data->selection < 0) return;

	game_sprite *owner_spr = self->parent->sprite;
	float old_x = self->sprite->x - get_x_offset(self);
	if (owner_spr->x < old_x) self->sprite->flip = SDL_FLIP_HORIZONTAL;
	else if (owner_spr->x > old_x) self->sprite->flip = SDL_FLIP_NONE;

	self->sprite->x = owner_spr->x + get_x_offset(self);
	self->sprite->y = owner_spr->y + 3;
//printf("owner_spr->x = %f, old_x = %f, self->sprite->x = %f")
	game_draw(self);
}

void weapon_attack(game_task *self, game_task *target) {
	weapon_data *data = self->data;
	game_adapter *adapter = target->adapter;
	mortal_hurt(target, adapter->mortality, data->types[data->selection]->attack_damage, true);
}

static void do_attack(game_task *self) {
	game_state *game = self->game;
	for (game_task *task = game->tasks.next; task != &game->tasks; task = task->next) {
		if (task->adapter == NULL || ((game_adapter *)task->adapter)->mortality == NULL || !game_is_touching_sprite(self, task->sprite)) continue;
		weapon_attack(self, task);
	}
}

static void handle_event(game_task *self) {
        weapon_data *state = self->data;
        game_services *svc = self->game->manager->data;

	if (self->parent->magic != PLAYER_MAGIC) return;

        if (!state->key_down) {
               if (self->game->event.type != SDL_EVENT_KEY_DOWN || ((SDL_KeyboardEvent *)&self->game->event)->key != svc->keys.attack) return;
                state->key_down = true;
                do_attack(self);
        } else {
                if (self->game->event.type != SDL_EVENT_KEY_UP || ((SDL_KeyboardEvent *)&self->game->event)->key != svc->keys.attack) return;
                state->key_down = false;
        }
}

