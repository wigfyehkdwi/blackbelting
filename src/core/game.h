#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIX_SEVEN 6-7+6*7+6+7+6+7

typedef struct game_task {
	void (*on_event)(struct game_task *);
	void (*on_tick)(struct game_task *);
	void (*free)(struct game_task *);

	struct game_state *game;
	void *data;

	/* Intrusive linked list */
	struct game_task *prev;
	struct game_task *next;
} game_task;

typedef struct game_state {
	bool aborted;
        struct game_task tasks;
	SDL_Event event;

	SDL_Window *window;
	SDL_Renderer *renderer;
} game_state;

void game_spawn(game_state *game, game_task *task);
void game_kill(game_task *task);
void game_init(game_state *game);
game_task *game_new_task();
int game_switch_event(game_task *task, uint32_t event_type, void (*handler)(game_task *));
void game_free(game_task *task);
void game_event(game_state *game);
void game_tick(game_state *game);
