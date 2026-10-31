#include "follow.h"
void follow_tick(game_task *self, game_task *target, float speed) {
	float sx = self->sprite->rect.x;
	float sy = self->sprite->rect.y;
	float tx = target->sprite->rect.x;
	float ty = target->sprite->rect.y;

	/* immitate tick from player.c */
	if (ty < sy) self->sprite->rect.y -= self->game->delta*speed;
	if (ty > sy) self->sprite->rect.y += self->game->delta*speed;
	if (tx < sx) self->sprite->rect.x -= self->game->delta*speed;
	if (tx > sx) self->sprite->rect.x += self->game->delta*speed;
}
