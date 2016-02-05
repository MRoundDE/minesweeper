#ifndef VIEW_H_
#define VIEW_H_

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "model.h"

// constants for the presentation
#define FIELD_WIDTH 64  // pixel
#define FIELD_HEIGHT 64 // pixel

#define BITMAP_PATH "assets/"

// Project global VIEW variables
extern SDL_Surface **symbols;
extern SDL_Window *window;

/**
 * Allocates memory for all data structures and ressources, used by the VIEW
 * and initializes them with appropriate values.
 */
void initialize_view(void);

/**
 * Performs all necessary steps to synchronize VIEW and MODEL.
 */
void update_view(void);

/**
 * Deallocates all memory used by the VIEW.
 */
void free_view(void);

/**
 * Translates display coordinates to field coordinates.
 */
void get_field_coordinates(SDL_Event e, int *x, int *y);

#endif
