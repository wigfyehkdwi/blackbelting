#include "dialogue.h"
#include "game_mgr.h"
#include <SDL3_image/SDL_image.h>

typedef struct {
	/* key states */
	bool up;
	bool down;
	bool left;
	bool right;
} dialogue_data;

static void handle_tick(game_task *self);

int dialogue(game_task *self) {
	self->on_tick = handle_tick;
	self->sprite = calloc(sizeof(game_sprite), 1);
	self->sprite->ui = true;
	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/game/dialogue.png");
	if (self->sprite->texture == NULL) return -1;
	game_scale_sprite(self->sprite, 3);

        game_task *mgr = self->game->manager;
        game_services *svc = mgr->data;
        svc->dialogue = self;

	return 0;
}

static void handle_tick(game_task *self) {
	game_draw(self);
}
