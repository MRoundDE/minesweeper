#include "controller.h"


void control_game(void) {
  SDL_Event e;
  int quit = 0;

  int mines_flagged_correct = 0;
  int mines_flagged_wrong = 0;
  int mines_unflagged = 0;

  // initial field drawing
  update_view();

  // MAIN loop
  while (!quit) {
    //  handle events on queue
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) { // user wants to quit
        quit = 1;
      } else if (e.type == SDL_MOUSEBUTTONDOWN) {       // user pressed mouse button
        int x = 0;
        int y = 0;
        get_field_coordinates(e, &x, &y);
        set_mines(x, y);

        // update model by given task
        if (e.button.button == SDL_BUTTON_LEFT) {
          if (field_static[x][y] == MINE) {
            field_dynamic[x][y] = EXPLODE;
            show_mines();
            quit = 1;
          } else {
            field_select(x, y);
          }

        } else if (e.button.button == SDL_BUTTON_RIGHT) {
          field_flag(x, y);
        }
        // model is now up to date
        update_view();

        get_mine_statistic(&mines_flagged_correct, &mines_flagged_wrong,
                           &mines_unflagged);
        if ((mines_unflagged == 0) && (mines_flagged_wrong == 0)) {
          print_text("YOU WON!!!");
          printf("\n\nMines flagged correct: %d\n", mines_flagged_correct);
          printf("\nMines flagged wrong: %d\n", mines_flagged_wrong);
          printf("\nUnflagged mines: %d\n\n\n", mines_unflagged);
          quit = 1;
        } else if (quit == 1) {
          print_text("YOU LOST!!!");
        }
      }
    }
    SDL_Delay(10);
  }

  // wait for user to quit
  while (e.type != SDL_QUIT) {
    while (SDL_PollEvent(&e)) {
      SDL_Delay(10);
    }
  }
}


void recursive_select(int x, int y) {
  field_dynamic[x][y] = SELECTED;

  coord_t coords[] = {
    {.x = x  , .y = y-1}, // North
    {.x = x+1, .y = y-1}, // North-East
    {.x = x+1, .y = y  }, // East
    {.x = x+1, .y = y+1}, // South-East
    {.x = x  , .y = y+1}, // South
    {.x = x-1, .y = y+1}, // South-West
    {.x = x-1, .y = y  }, // West
    {.x = x-1, .y = y-1}  // North-West
  };

  for (int i = 0; i < 8; i++) {
    if ((coords[i].x >= 0) && (coords[i].x < get_field_size_x())
        && (coords[i].y >= 0) && (coords[i].y < get_field_size_y())) {
      field_select(coords[i].x, coords[i].y);
    }
  }
}


void field_select(int x, int y) {
  if ((field_dynamic[x][y] == HIDDEN) && (field_static[x][y] == EMPTY)) {
    recursive_select(x, y);
  } else if ((field_dynamic[x][y] == HIDDEN) && (field_dynamic[x][y] != FLAG)) {
    field_dynamic[x][y] = SELECTED;
  }
}


void field_flag(int x, int y) {
  if (field_dynamic[x][y] == HIDDEN) {
    field_dynamic[x][y] = FLAG;
  } else if (field_dynamic[x][y] == FLAG) {
    field_dynamic[x][y] = HIDDEN;
  }
}


void show_mines() {
  for (int x = 0; x < get_field_size_x(); x++) {
    for (int y = 0; y < get_field_size_y(); y++) {
      if ((field_static[x][y] == MINE) && (field_dynamic[x][y] != FLAG)
          && (field_dynamic[x][y] != EXPLODE)) {
        field_dynamic[x][y] = SELECTED;
      }
    }
  }
}


void get_mine_statistic(int *mines_flagged_correct, int *mines_flagged_wrong,
                        int *mines_unflagged) {
  for (int x = 0; x < get_field_size_x(); x++) {
    for (int y = 0; y < get_field_size_y(); y++) {
      if (field_static[x][y] == MINE) {
        if (field_dynamic[x][y] == FLAG) {
          *mines_flagged_correct = *mines_flagged_correct + 1;
        } else {
          *mines_unflagged = *mines_unflagged + 1;
        }
      } else if (field_dynamic[x][y] == FLAG) {
        *mines_flagged_wrong = *mines_flagged_wrong + 1;
      }
    }
  }
}
