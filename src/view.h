#ifndef VIEW_H_
#define VIEW_H_

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "model.h"

// constants for the presentation
#define FIELD_WIDTH 64  // pixel
#define FIELD_HEIGHT 64 // pixel

/**
 * Creates a rectangular field of size FIELD_WIDTH times FIELD_HEIGHT, filled
 * with "color" and optional a text in "text_color".
 *
 * @param text String to write in the field.  If text == NULL, only a blank
 *             field with "color" is created.
 * @param color Background color of the field.
 * @param text_color Font color of the optional text.
 */
SDL_Texture *create_field(const char *text, SDL_Color color,
                          SDL_Color text_color);

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
