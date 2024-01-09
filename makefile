CFLAGS = -Wall -Wextra -std=c18 -lncursesw -g
FILES = src/main.c src/game.c src/console.c
CFLAGS_DEBUG = -Wall -Wextra -std=c18 -lncursesw -g 

all:
	gcc ${FILES} ${CFLAGS} -o game
debug:
	gcc ${FILES} ${CFLAGS_DEBUG} -o game