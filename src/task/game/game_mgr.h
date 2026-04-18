#include "../../core/game.h"
#include "keymap.h"

int game_mgr(game_task *self);

typedef struct {
	key_mappings keys;
	game_task *player;
} game_services;
