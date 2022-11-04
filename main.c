/**
 * @brief Game in OX
 * @file main.c
 * @author koamer
 * @date 2022-10-20
 * */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>

#include "console.h"

#define _XOPEN_SOURCE_EXTENDED
#ifndef DEBUG_MODE
	#define DEBUG_MODE 0 
#endif

int main(void)
{
	srand(time(NULL));
	setlocale(LC_ALL, "");
	
	initscr();

	Application_info app;

	construct_application_info(&app);

	bool is_running = true;

	while (is_running)
	{
		start_game(app.contex.player, NUMBER_OF_PLAYER);
		#if DEBUG_MODE == 1
		draw_field(&app);
		#endif
		Cordinates cord = get_mouse_click_postion();
		printw("Coordinates of clicked mouse: X : %d  Y :%d ", cord.x, cord.y);
		getch();
		is_running = false;
	}
	destroy_application_info(&app);
	endwin();
	return 0;
}
