#include "view.h"

#define BITMAP_PATH "assets/"

// Not visible outside this unit
SDL_Window *window = NULL;
SDL_Surface **symbols = NULL;
TTF_Font *font_field = NULL;

SDL_Color COLOR_RED = {.r = 255, .g = 0, .b = 0, .a = 255};
SDL_Color COLOR_GREEN = {.r = 0, .g = 255, .b = 0, .a = 255};
SDL_Color COLOR_BLUE = {.r = 0, .g = 0, .b = 255, .a = 255};
SDL_Color COLOR_RED_DARK = {.r = 100, .g = 0, .b = 0, .a = 255};
SDL_Color COLOR_GREEN_DARK = {.r = 0, .g = 100, .b = 0, .a = 255};
SDL_Color COLOR_BLUE_DARK = {.r = 0, .g = 0, .b = 100, .a = 255};
SDL_Color COLOR_GRAY_DARK = {.r = 128, .g = 128, .b = 128, .a = 255};
SDL_Color COLOR_GRAY_LIGHT = {.r = 200, .g = 200, .b = 200, .a = 255};
SDL_Color COLOR_OCHRE = {.r = 200, .g = 100, .b = 0, .a = 255};
SDL_Color COLOR_PURPLE = {.r = 200, .g = 0, .b = 200, .a = 255};

SDL_Surface *create_field(const char *text, SDL_Color color,
                          SDL_Color text_color) {
  SDL_Surface *s = SDL_CreateRGBSurface(0, FIELD_WIDTH, FIELD_HEIGHT, 32, 0, 0, 0,
                                        0);
  SDL_FillRect(s, NULL, SDL_MapRGB(s->format, color.r, color.g, color.b));
  if (text != NULL) {
    SDL_Rect offset;
    offset.x = 20;
    offset.y = -10;
    SDL_BlitSurface(TTF_RenderText_Solid(font_field, text, text_color), NULL, s,
                    &offset);
  }
  return s;
}

void initialize_view(void) {
  int window_width = get_field_size_x() * FIELD_WIDTH;
  int window_height = get_field_size_y() * FIELD_HEIGHT;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL initialization error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  if (TTF_Init() < 0) {
    printf("TTF initialization error:: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }
  window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, window_width,
                            window_height, 0);
  if (window == NULL) {
    printf("SDL window creation error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  symbols = (SDL_Surface **) malloc(13 * sizeof(SDL_Surface *));
  if (window == NULL) {
    printf("SDL memory allocation error.\n");
    exit(EXIT_FAILURE);
  }

  font_field = TTF_OpenFont(BITMAP_PATH "FreeSansBold.ttf", 50);
  if (font_field == NULL) {
    printf("TTF creation error: %s\n", TTF_GetError());
  }

  symbols[EMPTY] = create_field(NULL, COLOR_GRAY_LIGHT, COLOR_GRAY_LIGHT);
  symbols[ONE] = create_field("1", COLOR_GRAY_LIGHT, COLOR_BLUE);
  symbols[TWO] = create_field("2", COLOR_GRAY_LIGHT, COLOR_GREEN);
  symbols[THREE] = create_field("3", COLOR_GRAY_LIGHT, COLOR_RED);
  symbols[FOUR] = create_field("4", COLOR_GRAY_LIGHT, COLOR_BLUE_DARK);
  symbols[FIVE] = create_field("5", COLOR_GRAY_LIGHT, COLOR_RED_DARK);
  symbols[SIX] = create_field("6", COLOR_GRAY_LIGHT, COLOR_GREEN_DARK);
  symbols[SEVEN] = create_field("7", COLOR_GRAY_LIGHT, COLOR_OCHRE);
  symbols[EIGHT] = create_field("8", COLOR_GRAY_LIGHT, COLOR_PURPLE);
  symbols[MINE] = SDL_LoadBMP(BITMAP_PATH "mine.bmp");
  symbols[EXPLODE] = SDL_LoadBMP(BITMAP_PATH "explode.bmp");
  symbols[FLAG] = SDL_LoadBMP(BITMAP_PATH "flag.bmp");
  symbols[HIDDEN] = create_field(NULL, COLOR_GRAY_DARK, COLOR_GRAY_DARK);

  for (int i = 0; i <= HIDDEN; i++) {
    if (symbols[i] == NULL) {
      printf("Cannot load bitmap %d\n", i);
    }
  }
}


void update_view(void) {
  SDL_Rect offset;
  SDL_Surface *screen = SDL_GetWindowSurface(window);

  // Blit symbols
  for (int x = 0; x < get_field_size_x(); x++) {
    for (int y = 0; y < get_field_size_y(); y++) {
      // by default print the field
      SDL_Surface *symbol = symbols[field_static[x][y]];

      // modify default by dynamic field status
      if (field_dynamic[x][y] != SELECTED) {
        symbol = symbols[field_dynamic[x][y]];
      }

      offset.x = FIELD_WIDTH * x;
      offset.y = FIELD_HEIGHT * y;
      SDL_BlitSurface(symbol, NULL, screen, &offset);
    }
  }

  // Draw grid
  Uint32 *screen_pixels = screen->pixels;
  int screen_width = get_field_size_x() * FIELD_WIDTH;
  int screen_height = get_field_size_y() * FIELD_HEIGHT;
  for (int x = FIELD_WIDTH; x < screen_width; x = x + FIELD_WIDTH) {
    for (int y = 0; y < screen_height; y++) {
      screen_pixels[y * screen_width + x] = SDL_MapRGB(screen->format, 0, 0, 0);
    }
  }
  for (int y = FIELD_HEIGHT; y < screen_height; y = y + FIELD_HEIGHT) {
    for (int x = 0; x < screen_width; x++) {
      screen_pixels[y * screen_width + x] = SDL_MapRGB(screen->format, 0, 0, 0);
    }
  }

  SDL_UpdateWindowSurface(window);
}


void free_view(void) {
  for (int i = 0; i <= HIDDEN; i++) {
    SDL_FreeSurface(symbols[i]);
  }
  SDL_DestroyWindow(window);
  TTF_CloseFont(font_field);
  TTF_Quit();
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
