#include "game.h"
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

static void game_nop(game_task *self);
static int game_inop(game_task *self);
static bool out_of_range(float val, float a, float b);
static int game_load_texture(game_task *task, char *filepath);

void game_link(game_task *task) {
	printf("linking task %p\n", task);
	game_state *game = task->game;
	game_task *prev = game->tasks.next;
	for (game_task *candidate = prev; candidate != &game->tasks; candidate = candidate->next) {
		if (candidate->z > task->z) break;
		prev = candidate;	
	}

	task->next = prev->next;
	prev->next = task;

	task->prev = prev;
	task->next->prev = task;
	printf("done linking!\n");
}

void game_unlink(game_task *task) {
	task->next->prev = task->prev;
	task->prev->next = task->next;
}

void game_shift(game_task *task, int z) {
	task->z = z;
	game_unlink(task);
	game_link(task);
}

int game_spawn(game_task *parent, game_task *task) {
	printf("spawning task...\n");
	if (task == NULL) return -127;
	task->parent = parent;
	task->game = parent->game;

	/* resolve staged textures */
	if (task->sprite != NULL && task->sprite->texture != NULL) {
		char *filepath = (void *)task->sprite->texture;
		int result = game_load_texture(task, filepath);
		printf("%s %d\n", filepath, result);
		free(filepath);
		if (result) return -126;
	}

	game_link(task);
	return task->on_spawn(task);
}

void game_kill(game_task *task) {
	game_unlink(task);
	task->free(task);
	free(task);
}

void game_killall(game_task *root) {
	game_state *game = root->game;

	/* recursively kill children */
	for (game_task *task = game->tasks.next; task != &game->tasks; task = task->next) {
		if (task->parent == root) game_killall(task);
	}
	/* avoid messing with our linked list, and do it! */
	if (root != &game->tasks) game_kill(root);
}

void game_init(game_state *game) {
	memset(game, 0, sizeof(*game));
	game->tasks.prev = &game->tasks;
	game->tasks.next = &game->tasks;
	game->tasks.game = game;
}

static void game_nop(game_task *self) {}
static int game_inop(game_task *self) { return 0; }

game_task *new_game_task() {
	game_task *task = calloc(sizeof(game_task), 1);
	if (task == NULL) return NULL;

	task->on_spawn = game_inop;
	task->on_event = game_nop;
	task->on_tick = game_nop;
	task->free = game_nop;
	return task;
}

game_sprite *new_game_sprite() {
	game_sprite *sprite = calloc(sizeof(game_sprite), 1);
	if (sprite == NULL) return NULL;
	sprite->flip = SDL_FLIP_NONE;
	return sprite;
}

int game_switch_event(game_task *task, uint32_t event_type, void (*handler)(game_task *)) {
	int flag = task->game->event.type == event_type;

	if (flag) handler(task);
	return flag;
}

void game_event(game_state *game) {
	for (game_task *task = game->tasks.next; task != &game->tasks; task = task->next) {
		/*printf("handling event for task %p\n", task);*/
		task->on_event(task);
	}
}

void game_tick(game_state *game) {
	int ticks = SDL_GetTicks();
	game->delta = ticks - game->ticks;
	game->ticks = ticks;
	for (game_task *task = game->tasks.next; task != &game->tasks; task = task->next) {
		/*printf("ticking task %p\n", task);*/
		task->on_tick(task);
	}
	SDL_RenderPresent(game->renderer);
}

int game_load_sprite(game_task *task, char *filepath) {
	if (task->sprite == NULL) {
		task->sprite = new_game_sprite();
		if (task->sprite == NULL) return -1;
	}

	if (task->game == NULL) {
		size_t copy_size = strlen(filepath) + 1;
		char *filepath_staged = malloc(copy_size);
		memcpy(filepath_staged, filepath, copy_size);
		*(void **)&task->sprite->texture = filepath_staged;
		return 0;
	}

	return game_load_texture(task, filepath);
}

static int game_load_texture(game_task *task, char *filepath) {
	task->sprite->texture = IMG_LoadTexture(task->game->renderer, filepath);
	return (task->sprite->texture == NULL) ? -1 : 0;
}

int game_draw(game_task *task) {
	game_sprite *sprite = task->sprite;
	game_state *game = task->game;

	SDL_Texture *texture = sprite->texture;
	SDL_FRect rect;
	rect.x = sprite->x;
	rect.y = sprite->y;
	rect.w = sprite->w;
	rect.h = sprite->h;

	if (!sprite->ui) {
		rect.x -= game->camera.x;
		rect.y -= game->camera.y;
	}

	return SDL_RenderTextureRotated(game->renderer, texture, NULL, &rect, sprite->angle, sprite->center, sprite->flip);
}

static bool out_of_range(float val, float a, float b) {
	return val < a || val > (a + b);
}

bool game_is_touching_rect(game_sprite *sprite, float x, float y, float w, float h) {
	if (out_of_range(x, sprite->x, sprite->w) && out_of_range(x + w, sprite->x, sprite->w)) return false;
	if (out_of_range(y, sprite->y, sprite->h) && out_of_range(y + h, sprite->y, sprite->h)) return false;
	return true;
}

int game_is_clicked(game_task *task) {
	game_sprite *sprite = task->sprite;
	game_state *game = task->game;
	SDL_Event *event = &game->event;

	int result;
	if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) result = 1;
	else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) result = 2;
	else return 0;

	SDL_MouseButtonEvent *mouse_event = (SDL_MouseButtonEvent *)event;
	int mx = mouse_event->x;
	int my = mouse_event->y;

	if (!sprite->ui) {
		mx += task->game->camera.x;
		my += task->game->camera.y;
	}

	return game_is_touching_pos(sprite, mx, my) ? result : 0;
}

bool game_is_touching_sprite(game_task *task, game_sprite *target) {
	game_sprite *sprite = task->sprite;
	float tx = target->x;
	float ty = target->y;

	if (target->ui != sprite->ui) {
		if (target->ui) {
			tx -= task->game->camera.x;
			ty -= task->game->camera.y;
		} else {
			tx += task->game->camera.x;
			ty += task->game->camera.y;
		}
	}

	return game_is_touching_rect(sprite, tx, ty, target->w - 1, target->h - 1);
}

bool game_is_touching_task(game_task *task, game_task *target) {
	game_sprite *sprite = task->sprite;
	SDL_FRect *rect;
	for (int i = 0; i < target->hitbox_count; i++) {
		rect = &target->hitboxes[i];
		if (game_is_touching_rect(sprite, rect->x + sprite->x, rect->y + sprite->y, rect->w - 1, rect->h - 1)) return true;
	}
	return false;
}

void game_scale_sprite(game_sprite *sprite, float mul) {
	sprite->w = sprite->texture->w * mul;
	sprite->h = sprite->texture->h * mul;
}
