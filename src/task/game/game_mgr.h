#ifndef GAME_MGR_H
#define GAME_MGR_H
#include "../../core/game.h"
#include "keymap.h"

game_task *game_mgr();

typedef struct {
	key_mappings keys;
	game_task *player;
	game_task *dialogue;
	game_task *level;
} game_services;
#endif
