#include "levels.h"

#include "level_1.h"

const level levels[] = {
	{ .create = new_level_1, .save = NULL, .load = NULL }
};
