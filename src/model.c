#include "model.h"

// Project global fields
state_t **field_static = NULL;
state_t **field_dynamic = NULL;


// Not visible outside this unit
int FIELD_SIZE_X = 0;
int FIELD_SIZE_Y = 0;
int NUMBER_OF_MINES = 0;
int reset = 0;
game_state_t game_state = NORMAL;


void initialize_model(int number_of_mines, int field_size_x, int field_size_y) {
  FIELD_SIZE_X = field_size_x;
  FIELD_SIZE_Y = field_size_y;
  NUMBER_OF_MINES = number_of_mines;

  // Allocate memory
  field_static = (state_t **) malloc(FIELD_SIZE_X * sizeof(state_t *));
  for (int x = 0; x < FIELD_SIZE_X; x++) {
    field_static[x] = (state_t *) malloc(FIELD_SIZE_Y * sizeof(state_t));
  }
  field_dynamic = (state_t **) malloc(FIELD_SIZE_X * sizeof(state_t *));
  for (int x = 0; x < FIELD_SIZE_X; x++) {
    field_dynamic[x] = (state_t *) malloc(FIELD_SIZE_Y * sizeof(state_t));
  }

  reset_model();
}


void reset_model() {
  reset = 1;
  game_state = NORMAL;
  for (int x = 0; x < FIELD_SIZE_X; x++) {
    for (int y = 0; y < FIELD_SIZE_Y; y++) {
      field_static[x][y] = EMPTY;
      field_dynamic[x][y] = HIDDEN;
    }
  }
}


void place_mines(int start_x, int start_y) {
  if (!reset) {
    return;
  }
  reset = 0;

  // Initialize pseudo number generator
  srand(time(NULL));

  // place mines and numbers
  int i = 0;
  while (i < NUMBER_OF_MINES) {
    int x = rand() % FIELD_SIZE_X;
    int y = rand() % FIELD_SIZE_Y;
    // Avoid first click mine
    if (!((start_x == x) && (start_y == y)) && (field_static[x][y] != MINE)) {
      field_static[x][y] = MINE;

      // add numbers to neighbours
      if ((x > 0) && (field_static[x - 1][y] != MINE)) {
        field_static[x - 1][y]++;
      }
      if ((x < (FIELD_SIZE_X - 1)) && (field_static[x + 1][y] != MINE)) {
        field_static[x + 1][y]++;
      }
      if ((y > 0) && (field_static[x][y - 1] != MINE)) {
        field_static[x][y - 1]++;
      }
      if ((y < (FIELD_SIZE_Y - 1)) && (field_static[x][y + 1] != MINE)) {
        field_static[x][y + 1]++;
      }
      if ((x > 0) && (y > 0) && (field_static[x - 1][y - 1] != MINE)) {
        field_static[x - 1][y - 1]++;
      }
      if ((x < (FIELD_SIZE_X - 1)) && (y > 0)
          && (field_static[x + 1][y - 1] != MINE)) {
        field_static[x + 1][y - 1]++;
      }
      if ((x > 0) && (y < (FIELD_SIZE_Y - 1))
          && (field_static[x - 1][y + 1] != MINE)) {
        field_static[x - 1][y + 1]++;
      }
      if ((x < (FIELD_SIZE_X - 1)) && (y < (FIELD_SIZE_Y - 1))
          && (field_static[x + 1][y + 1] != MINE)) {
        field_static[x + 1][y + 1]++;
      }
      i++;
    }
  }
}


void free_model(void) {
  // Deallocate memory
  if (field_static != NULL) {
    for (int x = 0; x < FIELD_SIZE_X; x++) {
      if (field_static[x] != NULL) {
        free(field_static[x]);
      }
    }
    free(field_static);
  }
  if (field_dynamic != NULL) {
    for (int x = 0; x < FIELD_SIZE_X; x++) {
      if (field_dynamic[x] != NULL) {
        free(field_dynamic[x]);
      }
    }
    free(field_dynamic);
  }
}


int get_field_size_x(void) {
  return FIELD_SIZE_X;
}


int get_field_size_y(void) {
  return FIELD_SIZE_Y;
}

game_state_t get_game_state () {
  int number_hidden_fields = 0;
  int number_flag_fields = 0;
  for (int x = 0; x < get_field_size_x(); x++) {
    for (int y = 0; y < get_field_size_y(); y++) {
      if (field_dynamic[x][y] == EXPLODE) {
        game_state = LOOSE;
      }
      // count relevant info for win
      if (field_dynamic[x][y] == FLAG) {
        number_flag_fields++;
      } else if (field_dynamic[x][y] == HIDDEN) {
        number_hidden_fields++;
      }
    }
  }
  if ((!reset) && (game_state != LOOSE)
      && (number_hidden_fields + number_flag_fields == NUMBER_OF_MINES)) {
    game_state = WIN;
  }

  return game_state;
}
