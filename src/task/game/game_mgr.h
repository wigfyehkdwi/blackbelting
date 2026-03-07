#include "../../core/game.h"
#include "keymap.h"

int game_mgr(game_task *self);

struct game_services {
	key_mappings *keymap;
}
