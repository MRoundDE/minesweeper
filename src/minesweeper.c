#include <stdio.h>
#include <stdlib.h>

#include "model.h"
#include "view.h"
#include "controller.h"

// constants for the game
#define FIELD_SIZE_X 8
#define FIELD_SIZE_Y 8
#define NUMBER_OF_MINES 10

#ifdef _WIN32
int main(int argc, char* argv[]) {
#else
int main() {
#endif

  initialize_model(NUMBER_OF_MINES, FIELD_SIZE_X, FIELD_SIZE_Y);
  initialize_view();

  control_game();

  free_view();
  free_model();

  return EXIT_SUCCESS;
}
