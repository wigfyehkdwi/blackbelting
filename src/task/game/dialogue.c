#include "dialogue.h"
#include "game_mgr.h"
#include <SDL3_image/SDL_image.h>

typedef struct {
	bool key_down;
	int len;
	int idx;
	char **text;
} dialogue_state;

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);
static void handle_event(game_task *self);
static void proceed(game_task *self);

game_task *dialogue(char **diag_text, int diag_len) {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;
	self->on_event = handle_event;

	dialogue_state *state = calloc(sizeof(dialogue_state), 1);
	state->key_down = true; /* HACK: guess the initial value should be true */
	state->len = diag_len;
	state->text = diag_text;
	self->data = state;
	return self;
}

static int handle_spawn(game_task *self) {
        game_task *mgr = self->game->manager;
        game_services *svc = mgr->data;
        svc->dialogue = self;
	return 0;
}

static void handle_tick(game_task *self) {
	dialogue_state *state = self->data;
	char *text = state->text[state->idx];
        int win_w;
        int win_h;
        SDL_GetWindowSize(self->game->window, &win_w, &win_h);


	SDL_FRect rect = {.x = 0, .y = win_h - 100, .w = win_w, .h = 100}; /* TODO: Unhardcode */
	SDL_SetRenderDrawColor(self->game->renderer, 0, 0, 17, 255);
	SDL_RenderFillRect(self->game->renderer, &rect);

	SDL_SetRenderDrawColor(self->game->renderer, 255, 255, 255, 255);
	SDL_RenderDebugText(self->game->renderer, 20, 480 - 100 + 20, text);
	SDL_RenderDebugText(self->game->renderer, win_w - 220 - 20, win_h - 20, "Press any key to continue...");
}

static void handle_event(game_task *self) {
	dialogue_state *state = self->data;
	/*game_services *svc = self->game->manager->data;*/

        if (!state->key_down) {
	       if (self->game->event.type != SDL_EVENT_KEY_DOWN /*|| ((SDL_KeyboardEvent *)&self->game->event)->key != svc->keys.proceed*/) return;
		state->key_down = true;
		proceed(self);
	} else {
		if (self->game->event.type != SDL_EVENT_KEY_UP /*|| ((SDL_KeyboardEvent *)&self->game->event)->key != svc->keys.proceed*/) return;
		state->key_down = false;
	}
}
static void proceed(game_task *self) {
	dialogue_state *state = self->data;
	if (++state->idx < state->len) return;

	game_services *svc = self->game->manager->data;
	svc->dialogue = NULL;
	game_kill(self);
}
