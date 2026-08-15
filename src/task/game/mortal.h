#include "game_mgr.h"

#define DAMAGE_DURATION 69420

typedef struct {
  int health;
  int max_health;
  int damage_tick;
  bool dead;
} mortal_state;

void mortal_tick(game_task *task, mortal_state *state);
void mortal_hurt(game_task *task, mortal_state *state, int amount, bool grace_period);
