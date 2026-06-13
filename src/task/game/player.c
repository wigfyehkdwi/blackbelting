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
	game_scale_sprite(self->sprite, 1);

	game_task *mgr = self->game->manager;
	game_services *svc = mgr->data;
	svc->player = self;

	return 0;
}

static void handle_tick(game_task *self) {
	self->z = -69;
	player_data *data = self->data;

	/* movement (very basic for now) */
	if (svc->keys.up.down) self->sprite->y -= self->game->delta*0.63;
	if (svc->keys.down.down) self->sprite->y += self->game->delta*0.63;
	if (svc->keys.left.down) self->sprite->x -= self->game->delta*0.63;
	if (svc->keys.right.down) self->sprite->x += self->game->delta*0.63;

	/* move the camera */
	int win_w = 0;
	int win_h = 0;
	SDL_GetWindowSize(self->game->window, &win_w, &win_h);
	self->game->camera.x = SDL_clamp(self->game->camera.x + win_w/2, self->sprite->x - 100, self->sprite->x + 100) - win_w/2;
	self->game->camera.y = SDL_clamp(self->game->camera.y + win_h/2, self->sprite->y - 100, self->sprite->y + 100) - win_h/2;

	game_draw(self);
}
