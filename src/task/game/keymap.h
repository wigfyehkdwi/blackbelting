#include "../../core/game.h"
int keymap(game_task *self);

typedef struct {
        /* movement */
        Uint32 up;
        Uint32 down;
        Uint32 left;
        Uint32 right;

        /* actions */
        Uint32 interact;
	Uint32 proceed;
	Uint32 attack;

	/* misc */
	Uint32 pause;
} key_mappings;
