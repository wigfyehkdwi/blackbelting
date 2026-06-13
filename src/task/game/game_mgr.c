#include "game_mgr.h"
#include "player.h"
#include "enemy.h"
#include "axolotl.h"

static void handle_tick(game_task *self);
static void handle_event(game_task *self);

int game_mgr(game_task *self) {
	self->on_tick = handle_tick;
	self->on_event = handle_event;

	game_services *svc = calloc(sizeof(game_services), 1);
	self->data = svc;
	/* hardcode keys for now */
	svc->keys.up.code = SDLK_UP;
	svc->keys.down.code = SDLK_DOWN;
	svc->keys.left.code = SDLK_LEFT;
	svc->keys.right.code = SDLK_RIGHT;
	svc->keys.interact.code = SDLK_SPACE;
	svc->keys.proceed.code = SDLK_SPACE; /* SDLK_RETURN; */

	/* become the game manager */
	self->game->manager = self;

	/* spawn sub-tasks */
	if (game_spawn(self, player)) return 1;
	if (game_spawn(self, enemy)) return 1;
	if (game_spawn(self, axolotl)) return 1;

	return 0;
}

static void handle_tick(game_task *self) {
	SDL_SetRenderDrawColor(self->game->renderer, 12, 214, 255, 255);
	SDL_RenderClear(self->game->renderer);

}

static void handle_key(game_keys *keys, Uint32 keycode, bool down) {
	/* HACK: Treat svc->keys as an array */
	key_state *_keys = (game_key *)keys;
	int len = sizeof(game_keys) / sizeof(key_state);
	for (int i = 0; i < len; i += sizeof(key_state)) {
		key_state *key = _keys[i];
		if (key->code != keycode) continue;
		key->down = down;
		return;
	}
}

static void handle_event(game_task *self) {
	if (self->game->event.type == SDL_EVENT_QUIT) {
		self->game->exit = true;
	}

	/* keys */
	if (self->game->event.type == SDL_EVENT_KEY_DOWN) handle_key(self->keys, ((SDL_KeyboardEvent *)&self->game->event)->key, true);
	if (self->game->event.type == SDL_EVENT_KEY_UP) handle_key(self->keys,  ((SDL_KeyboardEvent *)&self->game->event)->key, false);
}
