/**
 * @brief Main details in game
 * @file game.h
 * @author koamer
 * @date 2022-10-20
 * */

#ifndef GAME_H
#define GAME_H

#define TABLE 3
#define CORNER_NUM 4 
typedef enum 
{
	GAME_CHARACTER_X = 0,
	GAME_CHARACTER_O = 1,
	GAME_CHARACTER_NS = 2 // Not Selected 
} CHARACTER;

typedef struct 
{
	uint32_t x;
	uint32_t y;
} Cordinates;

typedef struct 
{
	bool is_empty;
	CHARACTER character;
	Cordinates corner_cord[CORNER_NUM];
} Field;

typedef struct
{
	unsigned int moves;
	CHARACTER picked;
} Player;

void construct_field(Field *field);
void construct_player(Player *player);

bool make_move(Player* player, Field *field);
bool check_is_game_over(Field field[TABLE][TABLE]);

void start_game(Player* player, size_t number_of_player);
#endif
