#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

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

#ifdef _WIN32
  HMODULE sdl2_dll = LoadLibrary("assets/SDL2.dll");
  if (! sdl2_dll) {
    printf("Failed to load assets/SDL2.dll");
  }
#endif

  initialize_model(NUMBER_OF_MINES, FIELD_SIZE_X, FIELD_SIZE_Y);
  initialize_view();

  control_game();

  free_view();
  free_model();

#ifdef _WIN32
  FreeLibrary(sdl2_dll);
#endif

  return EXIT_SUCCESS;
}
