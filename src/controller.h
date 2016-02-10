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
 * Helper function for field_select.  Should not be accessed without proper
 * checking.
 *
 * @param x The fields x-Coordinate
 * @param y The fields y-Coordinate
 */
void recursive_select(int x, int y);

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
void show_mines();

/**
 * Generates statistics about mines.
 *
 * @param mines_flagged_correct Number of correct flagged mines
 * @param mines_flagged_wrong Number of wrong flagged mines
 * @param mines_unflagged Number of unflagged mines
 */
void get_mine_statistic(int *mines_flagged_correct, int *mines_flagged_wrong,
                        int *mines_unflagged);

#endif
