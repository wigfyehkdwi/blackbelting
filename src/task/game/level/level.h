#ifndef LEVEL_H
#define LEVEL_H
#include "../game_mgr.h"

typedef struct {
	game_task *(*create)();
	int (*save)(game_task *self);
	int (*load)(game_task *self);
} level;
#endif
