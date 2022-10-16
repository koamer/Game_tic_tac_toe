/**
 * @brief Main details in game
 * @file game.h
 * @author koamer
 * @date 2020-09-02
 * */

#ifndef GAME_H
#define GAME_H

#include "console.h"

typedef enum 
{
	GAME_CHARACTER_X = 0,
	GAME_CHARACTER_O = 1,
	GAME_CHARACTER_NS = 2 // Not Selected 
} CHARACTER;

typedef struct 
{
	uint8_t x;
	uint8_t y;
} Cordinates;

typedef struct 
{
	bool is_empty;
	CHARACTER character;
	Cordinates cord;
} Field;

typedef struct
{
	unsigned int moves;
	CHARACTER picked;
} Player;

void construct_field(Field *field);
void construct_player(Player *player);

bool make_move(void);			// TODO
bool check_is_game_over(void);	// TODO

void start_game(Player* player, size_t number_of_player);
#endif
