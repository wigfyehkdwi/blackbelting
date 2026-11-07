#ifndef ADAPTER_H
#define ADAPTER_H
#include "mortal.h"

typedef struct {
	mortal_state *mortality;
	game_task *weapon;
} game_adapter;

game_adapter *new_game_adapter();
#endif
