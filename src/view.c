#include "view.h"

#define BITMAP_PATH "assets/"

// Not visible outside this unit
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture **symbols = NULL;
SDL_Texture **top_bar_symbols = NULL;
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


SDL_Texture *create_field(const char *text, SDL_Color color,
                          SDL_Color text_color) {
  SDL_Surface *s = SDL_CreateRGBSurface(0, FIELD_WIDTH, FIELD_HEIGHT, 32, 0, 0,
                                        0, 0);
  SDL_FillRect(s, NULL, SDL_MapRGB(s->format, color.r, color.g, color.b));
  if (text != NULL) {
    SDL_Rect offset = {.x = 20, .y = -10};
    SDL_BlitSurface(TTF_RenderText_Solid(font_field, text, text_color), NULL, s,
                    &offset);
  }
  SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  return t;
}


void initialize_view(void) {
  int window_width = get_field_size_x() * FIELD_WIDTH;
  int window_height = get_field_size_y() * FIELD_HEIGHT + TOP_BAR_HEIGHT;
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
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (window == NULL) {
    printf("SDL renderer creation error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  // There are "(HIDDEN + 1)" visible symbols
  symbols = (SDL_Texture **) malloc((HIDDEN + 1) * sizeof(SDL_Texture *));
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
  symbols[MINE] = SDL_CreateTextureFromSurface(renderer,
                  SDL_LoadBMP(BITMAP_PATH "mine.bmp"));
  symbols[EXPLODE] = SDL_CreateTextureFromSurface(renderer,
                     SDL_LoadBMP(BITMAP_PATH "explode.bmp"));
  symbols[FLAG] = SDL_CreateTextureFromSurface(renderer,
                  SDL_LoadBMP(BITMAP_PATH "flag.bmp"));
  symbols[HIDDEN] = create_field(NULL, COLOR_GRAY_DARK, COLOR_GRAY_DARK);

  top_bar_symbols = (SDL_Texture **) malloc(NUMBER_OF_WIN_STATES * sizeof(
                      SDL_Texture *));
  top_bar_symbols[NORMAL] = SDL_CreateTextureFromSurface(renderer,
                            SDL_LoadBMP(BITMAP_PATH "mround.bmp"));
  top_bar_symbols[WIN] = SDL_CreateTextureFromSurface(renderer,
                         SDL_LoadBMP(BITMAP_PATH "mround_appr.bmp"));
  top_bar_symbols[LOOSE] = SDL_CreateTextureFromSurface(renderer,
                           SDL_LoadBMP(BITMAP_PATH "explode.bmp"));

  for (int i = 0; i <= HIDDEN; i++) {
    if (symbols[i] == NULL) {
      printf("Cannot load bitmap %d\n", i);
    }
  }

  for (int i = 0; i < NUMBER_OF_WIN_STATES; i++) {
    if (top_bar_symbols[i] == NULL) {
      printf("Cannot load bitmap %d\n", i);
    }
  }
}


void update_view(void) {
  // Top bar
  SDL_Rect top_bar = {
    .x = 0,
    .y = 0,
    .w = get_field_size_x() * FIELD_WIDTH,
    .h = TOP_BAR_HEIGHT
  };
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer, &top_bar);

  // Render top symbol
  SDL_Rect offset = {.w = FIELD_WIDTH, .h = FIELD_HEIGHT};
  offset.x = (FIELD_WIDTH * (get_field_size_x() - 1)) / 2;
  offset.y = (TOP_BAR_HEIGHT - FIELD_HEIGHT) / 2;
  SDL_RenderCopy(renderer, top_bar_symbols[get_game_state()], NULL, &offset);

  // Render field symbols
  for (int x = 0; x < get_field_size_x(); x++) {
    for (int y = 0; y < get_field_size_y(); y++) {
      // by default print the field
      SDL_Texture *symbol = symbols[field_static[x][y]];

      // modify default by dynamic field status
      if (field_dynamic[x][y] != SELECTED) {
        if (field_dynamic[x][y] == FLAG_WRONG) {
          symbol = symbols[FLAG];
        } else {
          symbol = symbols[field_dynamic[x][y]];
        }
      }

      offset.x = FIELD_WIDTH * x;
      offset.y = FIELD_HEIGHT * y + TOP_BAR_HEIGHT;
      SDL_RenderCopy(renderer, symbol, NULL, &offset);

      // Cross out wrong flags
      if (field_dynamic[x][y] == FLAG_WRONG) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // red
        for (int i = 0; i < BOLD_LINE_THICKNESS; i++) {
          SDL_RenderDrawLine(renderer, offset.x, offset.y + i,
                             offset.x + FIELD_WIDTH - i,
                             offset.y + FIELD_HEIGHT);
          SDL_RenderDrawLine(renderer, offset.x + i, offset.y + FIELD_HEIGHT,
                             offset.x + FIELD_WIDTH, offset.y + i);
        }
      }
    }
  }

  // Draw grid
  int screen_width = get_field_size_x() * FIELD_WIDTH;
  int screen_height = get_field_size_y() * FIELD_HEIGHT;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  for (int x = FIELD_WIDTH; x < screen_width; x = x + FIELD_WIDTH) {
    SDL_RenderDrawLine(renderer, x, TOP_BAR_HEIGHT, x,
                       screen_height + TOP_BAR_HEIGHT);
  }
  for (int y = FIELD_HEIGHT + TOP_BAR_HEIGHT;
       y < screen_height + TOP_BAR_HEIGHT; y = y + FIELD_HEIGHT) {
    SDL_RenderDrawLine(renderer, 0, y, screen_width, y);
  }
  SDL_RenderPresent(renderer);
}


void free_view(void) {
  for (int i = 0; i <= HIDDEN; i++) {
    SDL_DestroyTexture(symbols[i]);
  }
  for (int i = 0; i < NUMBER_OF_WIN_STATES; i++) {
    SDL_DestroyTexture(top_bar_symbols[i]);
  }
  SDL_DestroyRenderer(renderer);
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
  while ((*y < get_field_size_y())
         && ((FIELD_HEIGHT * (*y) + TOP_BAR_HEIGHT) < e.button.y)) {
    *y = *y + 1;
  }
  *y = *y - 1;
}


void print_top_bar_text(const char *text) {
  SDL_Rect offset;
  TTF_SizeText(font_field, text, &offset.w, &offset.h);
  offset.x = (get_field_size_x() * FIELD_WIDTH - offset.w) / 2;
  offset.y = (TOP_BAR_HEIGHT - offset.h) / 2;
  SDL_Texture *t = SDL_CreateTextureFromSurface(renderer,
                   TTF_RenderText_Solid(font_field, text, COLOR_RED));
  SDL_RenderCopy(renderer, t, NULL, &offset);
  SDL_RenderPresent(renderer);
}
