CFLAGS = -Wall -Wextra -std=c18 -lncursesw -g
FILES = main.c game.c console.c
CFLAGS_DEBUG = -Wall -Wextra -std=c18 -lncursesw -g -DDEBUG_MODE=1

all:
	gcc ${FILES} ${CFLAGS} -o game
debug:
	gcc ${FILES} ${CFLAGS_DEBUG} -o game