#include "adapter.h"

game_adapter *new_game_adapter() {
	return calloc(sizeof(game_adapter), 1);
}
