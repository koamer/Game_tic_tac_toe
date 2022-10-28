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
		draw_field(&app);
		getch();

		is_running = false;
	}
	destroy_application_info(&app);
	endwin();
	return 0;
}
