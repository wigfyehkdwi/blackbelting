#include "game.h"

int game_spawn(game_state *game, game_task *task) {
	game->tasks.prev->next = task;
	task->prev = game->tasks.prev;

	game->tasks.prev = task;
	task->next = &game->tasks;

	task->game = game;

	return task->on_spawn(task);
}

void game_kill(game_task *task) {
	task->game->tasks.prev = task->prev;
	task->game->tasks.next = task->next;

	task->free(task);
}

void game_init(game_state *game) {
	memset(game, 0, sizeof(*game));
	game->tasks.prev = &game->tasks;
	game->tasks.next = &game->tasks;
}

void game_nop(game_task *self) {
}
int game_nopi(game_task *self) {
	return 0;
}

static void game_default_free(game_task *self);
static void game_default_free(game_task *self) {
	free(self);
}

game_task *game_new_task() {
	game_task *task = calloc(sizeof(game_task), 1);
	if (task == NULL) return NULL;

	task->on_spawn = game_nopi;
	task->on_event = game_nop;
	task->on_tick = game_nop;
	task->free = game_default_free;
	return task;
}

game_sprite *game_new_sprite() {
	game_sprite *sprite = calloc(sizeof(game_sprite), 1);
	return sprite;
}

int game_switch_event(game_task *task, uint32_t event_type, void (*handler)(game_task *)) {
	int flag = task->game->event.type == event_type;

	if (flag) handler(task);
	return flag;
}

void game_event(game_state *game) {
	for (game_task *task = game->tasks.next; task != &game->tasks; task = task->next) {
		task->on_event(task);
	}
}

void game_tick(game_state *game) {
	int ticks = SDL_GetTicks();
	game->delta = ticks - game->ticks;
	game->ticks = ticks;
	for (game_task *task = game->tasks.next; task != &game->tasks; task = task->next) {
		task->on_tick(task);
	}
	SDL_RenderPresent(game->renderer);
}

int game_draw(game_task *task) {
	game_sprite *sprite = task->sprite;
	game_state *game = task->game;

	SDL_Texture *texture = sprite->texture;
	SDL_FRect rect;
	rect.x = sprite->x;
	rect.y = sprite->y;
	rect.w = texture->w;
	rect.h = texture->h;

	if (!sprite->ui) {
		rect.x -= game->camera.x;
		rect.y -= game->camera.y;
	}

	return SDL_RenderTexture(game->renderer, texture, NULL, &rect);
}

bool game_is_touching(game_sprite *sprite, int x, int y) {
	if (x < sprite->x || x > (sprite->x + sprite->texture->w)) return false;
	if (y < sprite->y || y > (sprite->y + sprite->texture->h)) return false;
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

	return game_is_touching(sprite, mx, my) ? result : 0;
}
