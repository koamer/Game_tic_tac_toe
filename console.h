/**
 * @brief Control console surface
 * @file console.h
 * @author koamer
 * @date 2022-10-20
 * */
#ifndef CONSOLE_H
#define CONSOLE_H
#include "game.h"
#include <ncurses.h>
#include <curses.h>

#include <menu.h>
#include <stdint.h>

#define MAX_COLOR_SETS 64
#define NUMBER_OF_PLAYER 2 
#define TABLE 3 

typedef struct Application_info
{
	struct Game_Contex 
	{
		Field field[TABLE][TABLE];
		Player player[NUMBER_OF_PLAYER];
		Player* current_player_move; 
	} contex;

	struct Set_of_colors
	{
		int16_t set_number;
		int16_t foreground_color_value;
		int16_t background_color_value;
	} set[MAX_COLOR_SETS];

	ITEM* it;
	FILE* logs;
	MENU* menu;
	
	uint32_t max_size_y;
	uint32_t max_size_x;
	
	uint32_t current_color;
} Application_info;

void construct_application_info(Application_info *app);
void destroy_application_info(Application_info *app);
void set_atribiute(Application_info *app, int32_t arguments, ...);
void set_color(Application_info *app, int16_t number_of_set);				
void create_set_of_colors(Application_info *app, uint8_t background_color, 	
							uint8_t foreground_color); 						
void create_logs_file(Application_info *app);									
void write_logs(Application_info *app, const char* message, const char* func);

Cordinates get_mouse_click_postion(Application_info *app); 

void draw_field(Application_info *app);
#endif