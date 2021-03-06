#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <SDL2/SDL.h>

#include "model.h"
#include "view.h"

/**
 * Start the main game loop.
 */
void control_game(void);


/**
 * Marks a field as "selected".  If the field is empty, all sourrounding fields
 * are marked as "selected" as well.
 *
 * @param x The fields x-Coordinate
 * @param y The fields y-Coordinate
 */
void field_select(int x, int y);

/**
 * Marks a field as "flagged".
 *
 * @param x The fields x-Coordinate
 * @param y The fields y-Coordinate
 */
void field_flag(int x, int y);

/**
 * Uncovers all mines on the field.
 */
void reveal_everything();

#endif
