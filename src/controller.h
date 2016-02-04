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
 * Marks a field as "selected".
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
 * Marks a field as "exploded" and uncovers all remaining mines.  Prints out a
 * loose message.
 *
 * @param x The fields x-Coordinate
 * @param y The fields y-Coordinate
 */
void field_explode(int x, int y);

/**
 * Checks necessary conditions to win.  Generates statistics about mines and
 * prints them out in case of a won game.
 */
int check_win(void);

#endif
