/**
 * @brief Implement of game
 * @file game.c
 * @author koamer
 * @date 2022-10-20
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

bool make_move(Player* player, Field * field, Cordinates *cord) {
	char current_move = player->picked == GAME_CHARACTER_O ? 'O' : 'X';

	return true;
}