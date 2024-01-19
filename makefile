CFLAGS = -Wall -Wextra -std=c18 -lncursesw
FILES = src/main.c src/game.c src/console.c
CFLAGS_DEBUG = -Wall -Wextra -std=c18 -lncursesw -g 

app:
	gcc ${FILES} ${CFLAGS} -o game
debug:
	gcc ${FILES} ${CFLAGS_DEBUG} -DDEBUG=1 -o game_dbg
clear:
	rm -rf game
	rm -rf game_dbg
	rm -rf logs 