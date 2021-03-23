CFLAGS = -Wall -Wextra -std=c18 -lncurses
FILES = main.c game.c console.c

all:
	gcc ${FILES} ${CFLAGS} -o game
