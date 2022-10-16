/**
 * @brief Game in OX
 * @file main.c
 * @author koamer
 * @date 2020-09-02
 * */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

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
			draw_field(&app, app.max_size_x - 1 , app.max_size_y );
			getch();

		is_running = false;
	}
	endwin();
	return 0;
}
