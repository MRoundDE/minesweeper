#ifndef MODEL_H_
#define MODEL_H_

#include <stdlib.h>
#include <time.h>

typedef enum field_state {
  // static states
  EMPTY = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, MINE,
  // dynamic states
  EXPLODE, FLAG, HIDDEN, SELECTED
} state_t;

extern state_t **field_static;
extern state_t **field_dynamic;

void initialize_model(int number_of_mines, int field_size_x,
                      int field_size_y);
void free_model(void);

int get_field_size_x(void);
int get_field_size_y(void);

#endif
