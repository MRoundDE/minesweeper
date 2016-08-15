#ifndef MODEL_H_
#define MODEL_H_

#include <stdlib.h>
#include <time.h>

typedef struct {
  int x;
  int y;
} coord_t;

typedef enum {
  // static states
  EMPTY = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, MINE,
  // dynamic states
  EXPLODE, FLAG, HIDDEN, SELECTED, FLAG_WRONG, NUMBER_OF_FIELD_STATES
} state_t;

// Project global fields
extern state_t **field_static;
extern state_t **field_dynamic;

/**
 * Allocates memory for all data structures used by the MODEL and initializes
 * them with appropriate values.
 *
 * @param number_of_mines The numbers of mines to be placed randomly
 * @param field_size_x Number of horizontal fields
 * @param field_size_y Number of vertical fields
 */
void initialize_model(int number_of_mines, int field_size_x, int field_size_y);

/**
 * Initialize all fields empty and hidden.
 */
void reset_model();

/**
 * Places randomly mines on the field, except for (x,y).
 *
 * @param x x-Coordinate to avoid
 * @param y y-Coordinate to avoid
 */
void place_mines(int x, int y);

/**
 * Deallocates all memory used by the MODEL.
 */
void free_model(void);

/**
 * Returns the number of horizontal fields.
 *
 * @return the number of horizontal fields
 */
int get_field_size_x(void);

/**
 * Returns the number of vertical fields.
 *
 * @return the number of vertical fields
 */
int get_field_size_y(void);

/**
 * Generates statistics about mines.
 *
 * @param mines_flagged_wrong Number of wrong flagged mines
 * @param mines_unflagged Number of unflagged mines
 */
void get_mine_statistic(int *mines_flagged_wrong, int *mines_unflagged);

#endif
