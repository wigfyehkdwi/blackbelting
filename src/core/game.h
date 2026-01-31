#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIX_SEVEN 6-7+6*7+6+7+6+7

typedef struct game_sprite {
	int x;
	int y;
	SDL_Texture *texture;
	bool ui;
} game_sprite;

typedef struct game_camera {
	int x;
	int y;
} game_camera;

typedef struct game_task {
	int z;

	int (*on_spawn)(struct game_task *);
	void (*on_event)(struct game_task *);
	void (*on_tick)(struct game_task *);
	void (*free)(struct game_task *);

	struct game_state *game;
	game_sprite *sprite;
	void *data;

	/* Intrusive linked list */
	struct game_task *prev;
	struct game_task *next;
} game_task;

typedef struct game_state {
	bool aborted;
	struct {
		game_task *first;
		game_task *last;
	} tasks;
	SDL_Event event;
	int ticks;
	int delta;
	game_camera camera;

	SDL_Window *window;
	SDL_Renderer *renderer;
} game_state;

void game_link(game_task *task);
void game_unlink(game_task *task);
int game_spawn(game_state *game, game_task *task);
void game_kill(game_task *task);
void game_init(game_state *game);
game_task *game_new_task();
game_sprite *game_new_sprite();
int game_switch_event(game_task *task, uint32_t event_type, void (*handler)(game_task *));
void game_free(game_task *task);
void game_event(game_state *game);
void game_tick(game_state *game);
int game_draw(game_task *task);
bool game_is_touching(game_sprite *sprite, int x, int y);
int game_is_clicked(game_task *task);
