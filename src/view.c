#include "view.h"

SDL_Window *window = NULL;
SDL_Surface **symbols = NULL;

void initialize_view(void) {
  int window_width = get_field_size_x() * FIELD_WIDTH;
  int window_height = get_field_size_y() * FIELD_HEIGHT;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL initialization error: %s\n", SDL_GetError());
  }
  window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, window_width,
                            window_height, 0);
  if (window == NULL) {
    printf("SDL window creation error: %s\n", SDL_GetError());
  }

  symbols = (SDL_Surface **) malloc(13 * sizeof(SDL_Surface *));
  if (window == NULL) {
    printf("SDL memory allocation error.\n");
    exit(EXIT_FAILURE);
  }

  symbols[EMPTY] = SDL_LoadBMP(BITMAP_PATH "empty.bmp");
  symbols[ONE] = SDL_LoadBMP(BITMAP_PATH "1.bmp");
  symbols[TWO] = SDL_LoadBMP(BITMAP_PATH "2.bmp");
  symbols[THREE] = SDL_LoadBMP(BITMAP_PATH "3.bmp");
  symbols[FOUR] = SDL_LoadBMP(BITMAP_PATH "4.bmp");
  symbols[FIVE] = SDL_LoadBMP(BITMAP_PATH "5.bmp");
  symbols[SIX] = SDL_LoadBMP(BITMAP_PATH "6.bmp");
  symbols[SEVEN] = SDL_LoadBMP(BITMAP_PATH "7.bmp");
  symbols[EIGHT] = SDL_LoadBMP(BITMAP_PATH "8.bmp");
  symbols[MINE] = SDL_LoadBMP(BITMAP_PATH "mine.bmp");
  symbols[EXPLODE] = SDL_LoadBMP(BITMAP_PATH "explode.bmp");
  symbols[FLAG] = SDL_LoadBMP(BITMAP_PATH "flag.bmp");
  symbols[HIDDEN] = SDL_LoadBMP(BITMAP_PATH "hidden.bmp");

  for (int i = 0; i <= HIDDEN; i++) {
    if (symbols[i] == NULL) {
      printf("Cannot load bitmap %d\n", i);
    }
  }
}

void update_view(void) {
  SDL_Rect offset;
  SDL_Surface *symbol;
  SDL_Surface *screen;

  screen = SDL_GetWindowSurface(window);

  for (int x = 0; x < get_field_size_x(); x++) {
    for (int y = 0; y < get_field_size_y(); y++) {
      // by default print the field
      symbol = symbols[field_static[x][y]];

      // modify default by dynamic field status
      if (field_dynamic[x][y] != SELECTED) {
        symbol = symbols[field_dynamic[x][y]];
      }
      // apply bitmap
      offset.x = FIELD_WIDTH * x;
      offset.y = FIELD_HEIGHT * y;
      SDL_BlitSurface(symbol, NULL, screen, &offset);
    }
  }
  SDL_UpdateWindowSurface(window);
}

void free_view(void) {
  // free allocated memory
  for (int i = 0; i <= HIDDEN; i++) {
    SDL_FreeSurface(symbols[i]);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
}

void get_field_coordinates(SDL_Event e, int *x, int *y) {
  *x = 0;
  *y = 0;
  while ((*x < get_field_size_x()) && ((FIELD_WIDTH * (*x)) < e.button.x)) {
    *x = *x + 1;
  }
  *x = *x - 1;
  while ((*y < get_field_size_y()) && ((FIELD_HEIGHT * (*y)) < e.button.y)) {
    *y = *y + 1;
  }
  *y = *y - 1;
}
