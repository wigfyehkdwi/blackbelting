#include "enemy.h"
#include "game_mgr.h"
#include "dialogue.h"
#include <SDL3_image/SDL_image.h>

enum enemy_phase {
	IDLE,
	DIALOGUE,
	FIGHT,
	DEAD
};

typedef struct {
	enum enemy_phase phase;
} enemy_data;

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);

static char *diag_text[] = {
	"hey i wanna fight!"
};

game_task *enemy() {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;
	self->data = calloc(sizeof(enemy_data), 1);
	if (self->data == NULL) return NULL;
	return self;
}

static int handle_spawn(game_task *self) {
	self->sprite = calloc(sizeof(game_sprite), 1);
	self->sprite->y = SIX_SEVEN;
	if (self->sprite == NULL) return -1;
	self->sprite->texture = IMG_LoadTexture(self->game->renderer, "res/game/enemy.png");
	if (self->sprite->texture == NULL) return -1;
	game_scale_sprite(self->sprite, 1);

	return 0;
}

static void handle_tick(game_task *self) {
	game_draw(self);

	enemy_data *state = self->data;
	game_task *mgr = self->game->manager;
	game_services *svc = mgr->data;
	if (state->phase == IDLE && svc->dialogue == NULL && game_is_touching_sprite(self, svc->player->sprite)) {
		game_spawn(mgr, dialogue(diag_text, 1));
		state->phase++;
	}
}
