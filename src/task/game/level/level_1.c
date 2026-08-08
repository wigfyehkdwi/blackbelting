#include "level_1.h"
#include "../player.h"
#include "../enemy.h"
#include "../axolotl.h"

static int handle_spawn(game_task *self);

game_task *new_level_1() {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;

	self->on_spawn = handle_spawn;
	return self;
}

static int handle_spawn(game_task *self) {
	game_services *svc = self->game->manager->data;
	svc->level = self;

	if (game_spawn(self, player())) return -1;
	if (game_spawn(self, enemy())) return -1;
	if (game_spawn(self, axolotl())) return -1;
	return 0;
}

/*int save_to_disk(FILE *dest) {
	fprintf(dest, "%d", enemy_data->state);
}

int load_from_disk(FILE *dest) {
	fscanf(dest, "%d", &enemy_data->state);
}*/
