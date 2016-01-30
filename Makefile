CC ?= gcc

TARGET = minesweeper

CFLAGS = -std=c11 -pedantic -Wall -Wextra
LDFLAGS = -lSDL2

all:
	$(CC) $(CFLAGS) -o $(TARGET) src/minesweeper.c $(LDFLAGS)

clean:
	rm -f $(TARGET)
