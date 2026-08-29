#include "follow.h"
void follow_tick(game_task *self, game_task *target, float speed) {
	int sx = self->sprite->x;
	int sy = self->sprite->y;
	int tx = target->sprite->x;
	int ty = target->sprite->y;

	/* immitate tick from player.c */
	if (ty < sy) self->sprite->y -= self->game->delta*speed;
	if (ty > sy) self->sprite->y += self->game->delta*speed;
	if (tx < sx) self->sprite->x -= self->game->delta*speed;
	if (tx > sx) self->sprite->x += self->game->delta*speed;
}
