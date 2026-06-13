#include "../../core/game.h"
int keymap(game_task *self);

typedef struct {
	Uint32 code;
	bool down;
} key_state;

typedef struct {
        /* movement */
        key_state up;
        key_state down;
        key_state left;
        key_state right;

        /* actions */
        key_state interact;
	key_state proceed;

        /* misc */
        key_state pause;
} game_keys;
