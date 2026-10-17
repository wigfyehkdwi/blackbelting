#include "game_mgr.h"

static int handle_spawn(game_task *self);
static void handle_tick(game_task *self);
static inline int clamp_coord(int coord, int area);

game_task *tiled_img(float x, float y, char* texture_path) {
	game_task *self = new_game_task();
	if (self == NULL) return NULL;
	self->on_spawn = handle_spawn;
	self->on_tick = handle_tick;

	if (game_load_sprite(self, texture_path)) return NULL;
	self->sprite->x = x;
	self->sprite->y = y;

	return self;
}

static int handle_spawn(game_task *self) {
	game_scale_sprite(self->sprite, 1);
	return 0;
}

static void handle_tick(game_task *self) {
	int win_w, win_h;
	if (!SDL_GetWindowSize(self->game->window, &win_w, &win_h)) return;

	game_sprite *sprite = self->sprite;
	game_camera *cam = &self->game->camera;
	SDL_Texture *texture = sprite->texture;
	SDL_FRect rect;
	rect.w = sprite->w;
	rect.h = sprite->h;

	for (float y = -clamp_coord(cam->y, rect.h); ; y += rect.h) {
		for (float x = -clamp_coord(cam->x, rect.w); ; x += rect.w) {
			rect.x = x;
			rect.y = y;
			SDL_RenderTexture(self->game->renderer, texture, NULL, &rect);
			if (x >= win_w) break;
		}
		if (y >= win_h) break;
	}
}

static inline int clamp_coord(int coord, int area) {
	coord = coord % area;
	if (coord < 0) coord = area + coord;
	return coord;
}
