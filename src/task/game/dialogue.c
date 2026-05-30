#include "dialogue.h"
#include "game_mgr.h"
#include <SDL3_image/SDL_image.h>

typedef struct {
	/* key states */
	bool up;
	bool down;
	bool left;
	bool right;
} dialogue_data;

static void handle_tick(game_task *self);
static void handle_event(game_task *self);

int dialogue(game_task *self) {
	self->on_tick = handle_tick;
	self->on_event = handle_event;

        game_task *mgr = self->game->manager;
        game_services *svc = mgr->data;
        svc->dialogue = self;

	return 0;
}

static void handle_tick(game_task *self) {
	SDL_FRect rect = {.x = 0, .y = 480 - 100, .w = 640, .h = 100}; /* TODO: Unhardcode */
	SDL_SetRenderDrawColor(self->game->renderer, 0, 0, 17, 255);
	SDL_RenderFillRect(self->game->renderer, &rect);

	SDL_SetRenderDrawColor(self->game->renderer, 255, 255, 255, 255);
	SDL_RenderDebugText(self->game->renderer, 20, 480 - 100 + 20, "testing...");
	SDL_RenderDebugText(self->game->renderer, 640 - 220 - 20, 480 - 20, "Press any key to continue...");
}

static void handle_event(game_task *self) {
	game_services *svc = self->game->manager->data;

        if (self.data) {
	       if (self->game->event.type != SDL_EVENT_KEY_DOWN || ((SDL_KeyboardEvent *)&self->game->event)->key != svc->keys.proceed) return;
		self->data = true;
		proceed(self);
	} else {
		if (self->game->event.type != SDL_EVENT_KEY_UP || ((SDL_KeyboardEvent *)&self->game->event)->key != svc->keys.proceed) return;
		self->data = false;
	}
}
static void proceed(game_task *self) {
	/* for now just terminate this task */
	game_services *svc = self->game->manager->data;
	svc->dialogue = NULL;
	game_kill(self);
}
