#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <SDL2/SDL.h>

#include "model.h"
#include "view.h"

void control_game(void);
void field_select(int x, int y);
void field_flag(int x, int y);
void field_explode(int x, int y);
int check_win(void);

#endif
