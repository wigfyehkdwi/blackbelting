#include "game.h"
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

static void game_nop(game_task *self);
static int game_inop(game_task *self);
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
	printf("unlinking task %p\n", task);
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
		float *staged = (float *)task->sprite->texture;

		float scale_x = *(staged + 0);
		float scale_y = *(staged + 1);

		char *filepath = (char *)(staged + 2);
		int result = game_load_texture(task, filepath);
		if (scale_x != 0.0) task->sprite->rect.w = task->sprite->texture->w * scale_x;
		if (scale_y != 0.0) task->sprite->rect.h = task->sprite->texture->h * scale_y;

		free(staged);
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
	game->manager->on_tick(game->manager);
	SDL_RenderPresent(game->renderer);
}

void game_tick_all(game_task *self) {
	game_state *game = self->game;
	for (game_task *task = game->tasks.next; task != &game->tasks; task = task->next) {
		if (task == self) continue; /* Avoid infinite recursion */
		task->on_tick(task);
	}
}

int game_load_sprite(game_task *task, char *filepath) {
	if (task->sprite == NULL) {
		task->sprite = new_game_sprite();
		if (task->sprite == NULL) return -1;
	}

	if (task->game == NULL) {
		size_t copy_size = strlen(filepath) + 1;
		void *staged = malloc(copy_size + sizeof(float)*2);
		memset(staged, 0, sizeof(float)*2);

		char *filepath_staged = (char *)((float *)staged + 2);
		memcpy(filepath_staged, filepath, copy_size);
		*(void **)&task->sprite->texture = staged;
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
	rect.x = sprite->rect.x;
	rect.y = sprite->rect.y;
	rect.w = sprite->rect.w;
	rect.h = sprite->rect.h;

	if (!sprite->ui) {
		rect.x -= game->camera.x;
		rect.y -= game->camera.y;
	}

	return SDL_RenderTextureRotated(game->renderer, texture, NULL, &rect, sprite->angle, sprite->center, sprite->flip);
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
	SDL_FPoint point = {mouse_event->x, mouse_event->y};

	if (!sprite->ui) {
		point.x += task->game->camera.x;
		point.y += task->game->camera.y;
	}

	return SDL_PointInRectFloat(&point, &sprite->rect) ? result : 0;
}

bool game_is_touching_sprite(game_task *task, game_sprite *target) {
	game_sprite *sprite = task->sprite;
	SDL_FRect tr = {target->rect.x, target->rect.y, target->rect.w, target->rect.h};

	if (target->ui != sprite->ui) {
		if (target->ui) {
			tr.x -= task->game->camera.x;
			tr.y -= task->game->camera.y;
		} else {
			tr.x += task->game->camera.x;
			tr.y += task->game->camera.y;
		}
	}

	return SDL_HasRectIntersectionFloat(&sprite->rect, &tr);
}

#if 0
bool game_is_touching_task(game_task *task, game_task *target) {
	game_sprite *sprite = task->sprite;
	SDL_FRect *rect;
	for (int i = 0; i < target->hitbox_count; i++) {
		rect = &target->hitboxes[i];
		if (game_is_touching_rect(sprite, rect->x + sprite->rect.x, rect->y + sprite->rect.y, rect->w - 1, rect->h - 1)) return true;
	}
	return false;
}
#endif

void game_scale(game_task *task, float scale_x, float scale_y) {
	if (task->game == NULL) { /* Staged sprites */
		float *staged = (float *)task->sprite->texture;
		*staged = scale_x;
		*(staged + 1) = scale_y;
		
	}

	task->sprite->rect.w = task->sprite->texture->w * scale_x;
	task->sprite->rect.h = task->sprite->texture->h * scale_y;
}
