CC ?= gcc

TARGET = minesweeper

CFLAGS = -std=c11 -pedantic -Wall -Wextra
LDFLAGS = -lSDL2

# See http://astyle.sourceforge.net/astyle.html
ASTYLE_FLAGS = --style=google \
               --indent=spaces=2 \
               --verbose \
               --convert-tabs

SRCS = $(wildcard src/*.c)
HEADS = $(wildcard src/*.h)
OBJS = $(SRCS:.c=.o)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(TARGET) src/*.o

format-source-code:
	astyle $(ASTYLE_FLAGS) $(SRCS) $(HEADS)
