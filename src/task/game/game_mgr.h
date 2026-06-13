#include "../../core/game.h"
#include "keymap.h"

int game_mgr(game_task *self);

typedef struct {
	game_keys keys;
	game_task *player;
	game_task *dialogue;
} game_services;
