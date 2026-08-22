#include "../../core/game.h"

#define WEAPON_MAGIC 0x7765706e

enum attack_type {
	MELEE,
	RANGED
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
	bool key_down;
} weapon_data;

game_task *weapon(game_task *owner, weapon_type *type);
int apply_weapon(game_task *self, int idx);

extern weapon_type weapon_sword;
extern weapon_type weapon_gun;
