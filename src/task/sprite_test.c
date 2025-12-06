#include "../core/game.h"
#include "quit_handler.h"

#include <SDL3_image/SDL_image.h>
#include<stdio.h>
static void load_resources(game_task *self);
static void handle_tick(game_task *self);

game_task *sprite_test() {
	game_task *task = game_new_task();
	task->on_spawn = load_resources;
	task->on_tick = handle_tick;
	return task;
}

static void load_resources(game_task *self) {
	self->data = IMG_LoadTexture(self->game->renderer, "res/test.png");
}

static void handle_tick(game_task *self) {
	SDL_Texture *texture = self->data;
	SDL_FRect rect;
	rect.x = 6;
	rect.y = 9;
	rect.w = texture->w;
	rect.h = texture->h;
	printf("%d", SDL_RenderTexture(self->game->renderer, texture, NULL, &rect));
}
