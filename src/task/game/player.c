#include "player.h"
#include "game_mgr.h"
#include <SDL3_image/SDL_image.h>

typedef struct {
	/* key states */
	bool up;
	bool down;
	bool left;
	bool right;
} player_data;

static void handle_tick(game_task *self);
static void handle_event(game_task *self);
static void handle_key(player_data *data, key_mappings *keys, Uint32 key, bool state);

int player(game_task *self) {
	self->on_tick = handle_tick;
	self->on_event = handle_event;
	self->data = calloc(sizeof(player_data), 1);

	self->sprite = calloc(sizeof(game_sprite), 1);
	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/game/player.png");
	if (self->sprite->texture == NULL) return -1;

	return 0;
}

static void handle_tick(game_task *self) {
	player_data *data = self->data;

	/* movement (very basic for now) */
	if (data->up) self->sprite->y -= self->game->delta;
	if (data->down) self->sprite->y += self->game->delta;
	if (data->left) self->sprite->x -= self->game->delta;
	if (data->right) self->sprite->x += self->game->delta;

	/* move the camera */
	self->game->camera.x = SDL_clamp(self->game->camera.x, self->sprite->x - 100, self->sprite->x + 100);
	self->game->camera.y = SDL_clamp(self->game->camera.y, self->sprite->y - 100, self->sprite->y + 100);

	game_draw(self);
}

static void handle_event(game_task *self) {
	game_services *svc = self->game->manager->data;
	if (self->game->event.type == SDL_EVENT_KEY_DOWN) handle_key(self->data, &svc->keys, ((SDL_KeyboardEvent *)&self->game->event)->key, true);
	else if (self->game->event.type == SDL_EVENT_KEY_UP) handle_key(self->data, &svc->keys, ((SDL_KeyboardEvent *)&self->game->event)->key, false);
}

static void handle_key(player_data *data, key_mappings *keys, Uint32 key, bool state) {
	if (key == keys->up) data->up = state;
	else if (key == keys->down) data->down = state;
	else if (key == keys->left) data->left = state;
	else if (key == keys->right) data->right = state;
}
