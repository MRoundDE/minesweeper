#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

// constants for the game
#define FIELD_SIZE_X 8
#define FIELD_SIZE_Y 8
#define NUMBER_OF_MINES 10

// constants for the presentation
#define FIELD_WIDTH 64 // pixel
#define FIELD_HEIGHT 64 // pixel
#define WINDOW_WIDTH (FIELD_WIDTH * FIELD_SIZE_X) // pixel
#define WINDOW_HEIGHT (FIELD_HEIGHT * FIELD_SIZE_Y) // pixel

#define FIELD_OFFSET_X(x) (FIELD_WIDTH * (x))
#define FIELD_OFFSET_Y(y) (FIELD_HEIGHT * (y))

#define BITMAP_PATH "assets/"

enum field_state {
  // static states
  EMPTY = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, MINE,
  // dynamic states
  EXPLODE, FLAG, HIDDEN, SELECTED
};

// global MODEL variables
int field_static[FIELD_SIZE_X][FIELD_SIZE_Y];
int field_dynamic[FIELD_SIZE_X][FIELD_SIZE_Y];

// global VIEW variables
SDL_Surface* symbols[13];
SDL_Window* window;

int check_win() {
  int mines_flagged_correct = 0;
  int mines_flagged_wrong = 0;
  int mines_unflagged = 0;
  int x, y;

  // generate mine statistic
  for (x = 0; x < FIELD_SIZE_X; x++) {
    for (y = 0; y < FIELD_SIZE_Y; y++) {
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

void update_view() {
  int x, y;
  SDL_Rect offset;
  SDL_Surface* symbol;
  SDL_Surface* screen;

  screen = SDL_GetWindowSurface(window);

  for (x = 0; x < FIELD_SIZE_X; x++) {
    for (y = 0; y < FIELD_SIZE_Y; y++) {
      // by default print the field
      symbol = symbols[field_static[x][y]];

      // modify default by dynamic field status
      if (field_dynamic[x][y] != SELECTED) {
        symbol = symbols[field_dynamic[x][y]];
      }

      // apply bitmap
      offset.x = FIELD_OFFSET_X(x);
      offset.y = FIELD_OFFSET_Y(y);
      SDL_BlitSurface(symbol, NULL, screen, &offset);
    }
  }
  SDL_UpdateWindowSurface(window);
}

void initialize_model() {
  int x, y, i;

  // initialize all fields empty and hidden
  for (x = 0; x < FIELD_SIZE_X; x++) {
    for (y = 0; y < FIELD_SIZE_Y; y++) {
      field_static[x][y] = EMPTY;
      field_dynamic[x][y] = HIDDEN;
    }
  }

  // place mines and numbers
  i = 0;
  while (i < NUMBER_OF_MINES) {
    x = rand() % FIELD_SIZE_X;
    y = rand() % FIELD_SIZE_Y;
    if (field_static[x][y] != MINE) {
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
      if ((x < (FIELD_SIZE_X - 1)) && (y > 0) && (field_static[x + 1][y - 1] != MINE)) {
        field_static[x + 1][y - 1]++;
      }
      if ((x > 0) && (y < (FIELD_SIZE_Y - 1)) && (field_static[x - 1][y + 1] != MINE)) {
        field_static[x - 1][y + 1]++;
      }
      if ((x < (FIELD_SIZE_X - 1)) && (y < (FIELD_SIZE_Y - 1)) && (field_static[x + 1][y + 1] != MINE)) {
        field_static[x + 1][y + 1]++;
      }
      i++;
    }
  }
}

void initialize_view() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL initialization error: %s\n", SDL_GetError());
  }
  window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if(window == NULL) {
    printf("SDL window creation error: %s\n", SDL_GetError());
  }
  symbols[EMPTY]   = SDL_LoadBMP(BITMAP_PATH "empty.bmp");
  symbols[ONE]     = SDL_LoadBMP(BITMAP_PATH "1.bmp");
  symbols[TWO]     = SDL_LoadBMP(BITMAP_PATH "2.bmp");
  symbols[THREE]   = SDL_LoadBMP(BITMAP_PATH "3.bmp");
  symbols[FOUR]    = SDL_LoadBMP(BITMAP_PATH "4.bmp");
  symbols[FIVE]    = SDL_LoadBMP(BITMAP_PATH "5.bmp");
  symbols[SIX]     = SDL_LoadBMP(BITMAP_PATH "6.bmp");
  symbols[SEVEN]   = SDL_LoadBMP(BITMAP_PATH "7.bmp");
  symbols[EIGHT]   = SDL_LoadBMP(BITMAP_PATH "8.bmp");
  symbols[MINE]    = SDL_LoadBMP(BITMAP_PATH "mine.bmp");
  symbols[EXPLODE] = SDL_LoadBMP(BITMAP_PATH "explode.bmp");
  symbols[FLAG]    = SDL_LoadBMP(BITMAP_PATH "flag.bmp");
  symbols[HIDDEN]  = SDL_LoadBMP(BITMAP_PATH "hidden.bmp");

  for (int i = 0; i <= HIDDEN; i++) {
    if (symbols[i] == NULL) {
      printf("Cannot load bitmap %d\n", i);
    }
  }
}

int main(int argc, char* argv[]) {
  SDL_Event e;
  int quit = 0;
  int x, y, i, j;

  // Initialize pseudo number generator
  srand(time(NULL));

  initialize_model();
  initialize_view();
  update_view();

  // MAIN loop
  while (!quit) {
    //  handle events on queue
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) { // user wants to quit
        quit = 1;
      } else if (e.type == SDL_MOUSEBUTTONDOWN) { // user pressed mouse button
        // read in coordinates
        for (x = 0; ((x < FIELD_SIZE_X) && (FIELD_OFFSET_X(x) < e.button.x)); x++);
        x--;
        for (y = 0; ((y < FIELD_SIZE_Y) && (FIELD_OFFSET_Y(y) < e.button.y)); y++);
        y--;

        // update model by given task
        if (e.button.button == SDL_BUTTON_LEFT) {
          // select field
          if ((field_dynamic[x][y] == HIDDEN) && (field_dynamic[x][y] != FLAG)) {
            field_dynamic[x][y] = SELECTED;

            if (field_static[x][y] == MINE) {
              // select all mines to show them to the user
              for (i = 0; i < FIELD_SIZE_X; i++) {
                for (j = 0; j < FIELD_SIZE_Y; j++) {
                  if ((field_static[i][j] == MINE) && (field_dynamic[i][j] != FLAG)) {
                    field_dynamic[i][j] = SELECTED;
                  }
                }
              }
              field_dynamic[x][y] = EXPLODE;

              printf("\n\nYOU LOST!!!\n\n");
              quit = 1;
            } else {
              quit = check_win();
            }
          }
        } else if (e.button.button == SDL_BUTTON_RIGHT) {
          // flag field
          if (field_dynamic[x][y] == HIDDEN) {
            field_dynamic[x][y] = FLAG;
          } else if (field_dynamic[x][y] == FLAG) {
            field_dynamic[x][y] = HIDDEN;
          }
          quit = check_win();
        }

        // model is now up to date
        update_view();
      }
    }
    SDL_Delay (10);
  }

  // wait for user to quit
  while (e.type != SDL_QUIT) {
    while (SDL_PollEvent(&e)) {
      SDL_Delay (10);
    }
  }

  // free allocated memory
  for (i = 0; i <= HIDDEN; i++) {
    SDL_FreeSurface(symbols[i]);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
