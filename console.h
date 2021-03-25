/**
 * @brief Control console surface
 * @file console.h
 * @author koamer
 * @date 2020-09-02
 * */

#ifndef CONSOLE_H
#define CONSOLE_H

#include "game.h"
#include <ncurses.h>
#include <curses.h>
#include <menu.h>

#define MAX_COLOR_SETS 64
#define NUMBER_OF_PLAYER 2 
#define TABLE 3 

typedef struct Application_info
{
	ITEM* it;
	FILE* logs;
	MENU* menu;
	Field field[TABLE][TABLE];
	Player player[NUMBER_OF_PLAYER];

	struct Set_of_colors
	{
		short set_number;
		short foreground_color_value;
		short background_color_value;
	} set[MAX_COLOR_SETS];

	unsigned int max_size_x;
	unsigned int max_size_y;

	unsigned int current_x;
	unsigned int current_y;
	unsigned int current_color;

	

} Application_info;

void construct_application_info(Application_info *app);
void destroy_application_info(Application_info *app);
void set_atribiute(Application_info *app, int arguments, ...);
void set_color(Application_info *app, uint8_t number_of_set);				
void create_set_of_colors(Application_info *app, uint8_t background_color, 	
							uint8_t foreground_color ); 						
void create_logs_file(Application_info *app);									
void write_logs(Application_info *app, const char* message, const char* func);
Cordinates get_mouse_click_postion(Application_info *app); 		 

#endif