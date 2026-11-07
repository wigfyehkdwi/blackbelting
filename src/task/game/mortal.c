#include "mortal.h"

void mortal_tick(game_task *task, mortal_state *state) {
	if (task->game->ticks >= (state->damage_tick + DAMAGE_DURATION)) {
		if (state->health <= 0) state->dead = true;
		else SDL_SetTextureColorMod(task->sprite->texture, 255, 255, 255);
	}

	/* draw a health bar */
	SDL_FRect *tr = &task->sprite->rect;
	game_camera *cam = &task->game->camera;

	SDL_FRect rect = {tr->x - cam->x, tr->y - 5 - cam->y, tr->w - state->health/tr->w, 5};
	SDL_SetRenderDrawColor(task->game->renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(task->game->renderer, &rect);
}

void mortal_hurt(game_task *task, mortal_state *state, int amount, bool grace_period) {
	if (grace_period && task->game->ticks < (state->damage_tick + DAMAGE_DURATION)) return;
	state->health -= amount;
	state->damage_tick = task->game->ticks;
	SDL_SetTextureColorMod(task->sprite->texture, 255, 63, 63);
}

/*void mortal_check_weapon(game_task *task, )*/
