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

	void (*on_event)(struct game_task *);
	void (*on_tick)(struct game_task *);
	void (*free)(struct game_task *);

	struct game_state *game;
	struct game_task *parent;
	game_sprite *sprite;
	void *data;

	/* Intrusive linked list */
	struct game_task *prev;
	struct game_task *next;
} game_task;

typedef struct game_state {
	bool exit;
        struct game_task tasks;
	SDL_Event event;
	int ticks;
	int delta;
	game_camera camera;
	game_task *mgr;

	SDL_Window *window;
	SDL_Renderer *renderer;
} game_state;

void game_link(game_task *task);
void game_unlink(game_task *task);
void game_shift(game_task *task, int z);
int game_spawn(game_task *parent, int (*on_spawn)(game_task *self));
void game_kill(game_task *task);
void game_killall(game_task *root);
void game_init(game_state *game);
game_sprite *game_new_sprite();
int game_switch_event(game_task *task, uint32_t event_type, void (*handler)(game_task *));
void game_free(game_task *task);
void game_event(game_state *game);
void game_tick(game_state *game);
int game_draw(game_task *task);
bool game_is_touching(game_sprite *sprite, int x, int y);
int game_is_clicked(game_task *task);
