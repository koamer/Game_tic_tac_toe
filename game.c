/**
 * @brief Implement of game
 * @file game.c
 * @author koamer
 * @date 2023-11-01
 * */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>

#include "console.h"

void construct_field(Field *field)
{
	field->is_empty = true;
	field->character = GAME_CHARACTER_NS;

	for(uint32_t i = 0; i < CORNER_NUM; i++) {
		field->corner_cord[i].x = 0;
		field->corner_cord[i].y = 0;
	}
}	

void construct_player(Player *player)
{
	player->picked = GAME_CHARACTER_NS;
	player->moves = 0;
}

void start_game(Player* player, const size_t number_of_player)
{
	if(number_of_player != 2 ) {
		fprintf(stderr, "Error: %s More then 2 players",__func__);
		exit(EXIT_FAILURE);
	}
	unsigned int who_start = rand() % number_of_player;
	player[0].picked = (CHARACTER)who_start;
	player[1].picked = (CHARACTER)(!who_start);
}

bool check_is_game_over(Field field[TABLE][TABLE]) {
	(void)field;
	return true;
}

bool make_move(Player* player, Field *field) {
	const char current_move = player->picked == GAME_CHARACTER_O ? 'O' : 'X';
	UNUSED(current_move);
	UNUSED(field);
	return true;
}