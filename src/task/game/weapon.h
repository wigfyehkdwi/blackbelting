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
	weapon_type *type;
} weapon_data;

game_task *weapon(game_task *owner, weapon_type *type);
int apply_weapon(game_task *self, weapon_type *type);

extern weapon_type weapon_sword;
