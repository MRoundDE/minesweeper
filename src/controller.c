#include "controller.h"

void control_game(void) {
  SDL_Event e;
  int quit = 0;

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

        // update model by given task
        if (e.button.button == SDL_BUTTON_LEFT) {
          if (field_static[x][y] == MINE) {
            field_explode(x, y);
            quit = 1;
          } else {
            field_select(x, y);
            quit = check_win();
          }

        } else if (e.button.button == SDL_BUTTON_RIGHT) {
          field_flag(x, y);
          quit = check_win();
        }
        // model is now up to date
        update_view();
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

void field_select(int x, int y) {
  if ((field_dynamic[x][y] == HIDDEN) && (field_dynamic[x][y] != FLAG)) {
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

void field_explode(int x, int y) {
  // select all mines to show them to the user
  for (int i = 0; i < get_field_size_x(); i++) {
    for (int j = 0; j < get_field_size_y(); j++) {
      if ((field_static[i][j] == MINE) && (field_dynamic[i][j] != FLAG)) {
        field_dynamic[i][j] = SELECTED;
      }
    }
  }
  field_dynamic[x][y] = EXPLODE;

  printf("\n\nYOU LOST!!!\n\n");
}

int check_win(void) {
  int mines_flagged_correct = 0;
  int mines_flagged_wrong = 0;
  int mines_unflagged = 0;

  // generate mine statistic
  for (int x = 0; x < get_field_size_x(); x++) {
    for (int y = 0; y < get_field_size_y(); y++) {
      if (field_static[x][y] == MINE) {
        if (field_dynamic[x][y] == FLAG) {
          mines_flagged_correct++;
        } else {
          mines_unflagged++;
        }
      } else if (field_dynamic[x][y] == FLAG) {
        mines_flagged_wrong++;
      }
    }
  }

  if ((mines_unflagged == 0) && (mines_flagged_wrong == 0)) {
    printf("\n\nYOU WON!!!\n\n");
    printf("\n\nMines flagged correct: %d\n", mines_flagged_correct);
    printf("\nMines flagged wrong: %d\n", mines_flagged_wrong);
    printf("\nUnflagged mines: %d\n\n\n", mines_unflagged);
    return 1;
  }

  return 0;
}
