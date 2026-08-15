#include "mortal.h"

void mortal_tick(game_task *task, mortal_state *state) {
	if (task->game->ticks == (state->damage_tick + DAMAGE_DURATION)) {
		if (state->health <= 0) state->dead = true;
		else SDL_SetTextureColorMod(task->sprite->texture, 255, 255, 255);
	}
}

void mortal_hurt(game_task *task, mortal_state *state, int amount, bool grace_period) {
	if (grace_period && task->game->ticks < (state->damage_tick + DAMAGE_DURATION)) return;
	state->health -= amount;
	state->damage_tick = task->game->ticks;
	SDL_SetTextureColorMod(task->sprite->texture, 255, 63, 63);
}

/*void mortal_check_weapon(game_task *task, )*/
