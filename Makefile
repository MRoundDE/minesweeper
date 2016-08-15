CC ?= gcc

TARGET = minesweeper
VERSION = 0.2.0-alpha

ZIP_NAME = $(TARGET)-$(VERSION).zip

# i686-w64-mingw32
MINGW_ARCH ?= x86_64-w64-mingw32
SDL2_PATH_WIN ?= .
SDL2_TTF_PATH_WIN ?= .

# specify root directory (default: current directory)
ROOT_DIR ?= ${PWD}
TEMP_DIR := $(shell mktemp -d)

# Specify include, library, and dll paths relative to SDL2 archive.
SDL2_INC_WIN = -I$(SDL2_PATH_WIN)/$(MINGW_ARCH)/include \
               -I$(SDL2_PATH_WIN)/$(MINGW_ARCH)/include/SDL2
SDL2_LIB_WIN = -L$(SDL2_PATH_WIN)/$(MINGW_ARCH)/lib
SDL2_DLL_WIN = $(wildcard $(SDL2_PATH_WIN)/$(MINGW_ARCH)/bin/*.dll)
SDL2_LIC_WIN = $(SDL2_PATH_WIN)/README-SDL.txt

# Specify include, library, and dll paths relative to SDL2_TTF archive.
SDL2_TTF_INC_WIN = -I$(SDL2_TTF_PATH_WIN)/$(MINGW_ARCH)/include
SDL2_TTF_LIB_WIN = -L$(SDL2_TTF_PATH_WIN)/$(MINGW_ARCH)/lib
SDL2_TTF_DLL_WIN = $(wildcard $(SDL2_TTF_PATH_WIN)/$(MINGW_ARCH)/bin/*.dll)
SDL2_TTF_LIC_WIN = $(wildcard $(SDL2_TTF_PATH_WIN)/$(MINGW_ARCH)/bin/*.txt)

ifneq (,$(findstring mingw,$(CC)))
  PROG_NAME = $(TARGET).exe
else
  PROG_NAME = $(TARGET)
endif

ifneq (,$(findstring mingw,$(CC)))
  CFLAGS = -std=c99 -pedantic -Wall -Wextra $(SDL2_INC_WIN) $(SDL2_TTF_INC_WIN)
else
  CFLAGS = -std=c99 -pedantic -Wall -Wextra
endif

ifneq (,$(findstring mingw,$(CC)))
  LDFLAGS = $(SDL2_LIB_WIN) $(SDL2_TTF_LIB_WIN) \
            -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -mwindows
else
  LDFLAGS = -lSDL2 -lSDL2_ttf
endif

# See http://astyle.sourceforge.net/astyle.html
ASTYLE_FLAGS = --style=google \
               --indent=spaces=2 \
               --max-code-length=80 \
               --verbose \
               --convert-tabs

SRCS = $(wildcard src/*.c)
HEADS = $(wildcard src/*.h)
OBJS = $(SRCS:.c=.o)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(PROG_NAME)

$(PROG_NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG_NAME) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(PROG_NAME) $(ZIP_NAME) src/*.o src/*.orig src/*.gch

dist-win: $(PROG_NAME)
	mkdir -p $(TEMP_DIR)/license
	cp -t $(TEMP_DIR) $(PROG_NAME) README.md
	cp -R assets $(TEMP_DIR)
	cp -t $(TEMP_DIR) $(SDL2_DLL_WIN) $(SDL2_TTF_DLL_WIN)
	cp -t $(TEMP_DIR)/license $(SDL2_LIC_WIN) $(SDL2_TTF_LIC_WIN) LICENSE
	cd $(TEMP_DIR) && zip -r $(ROOT_DIR)/$(ZIP_NAME) *

format-source-code:
	astyle $(ASTYLE_FLAGS) $(SRCS) $(HEADS)
