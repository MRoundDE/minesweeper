#ifndef VIEW_H_
#define VIEW_H_

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "model.h"

// constants for the presentation
#define FIELD_WIDTH 64  // pixel
#define FIELD_HEIGHT 64 // pixel

#define BITMAP_PATH "assets/"

// VIEW variables
extern SDL_Surface **symbols;
extern SDL_Window *window;

void initialize_view(void);
void update_view(void);
void free_view(void);
void get_field_coordinates(SDL_Event e, int *x, int *y);

#endif
