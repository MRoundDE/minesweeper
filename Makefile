CC ?= gcc

TARGET = minesweeper

CFLAGS = -std=c11 -pedantic -Wall -Wextra
LDFLAGS = -lSDL2

# See https://www.gnu.org/software/indent/manual/html_node/indent_17.html
INDENT_FLAGS = --k-and-r-style \
               --indent-level2 \
               --braces-on-func-def-line \
               --braces-on-struct-decl-line \
               --blank-lines-after-procedures \
               --comment-indentation0 \
               --declaration-comment-column0 \
               --else-endif-column0 \
               --verbose  \
               --no-tabs

SRCS = $(wildcard src/*.c)
HEADS = $(wildcard src/*.h)
OBJS = $(SRCS:.c=.o)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(TARGET) src/*.o

indent:
	indent $(INDENT_FLAGS) $(SRCS) $(HEADS)
