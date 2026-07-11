#include "../../core/game.h"

enum attack_type {
	MELEE
};

typedef struct {
	enum attack_type type;
	char *sprite_path;
	float scale;
} weapon_type;

typedef struct {
	game_task *owner;
	weapon_type *types[9];
	int selection;
} weapon_data;

game_task *weapon(game_task *owner, weapon_type *type);
int apply_weapon(game_task *self, int idx);

extern weapon_type weapon_sword;
