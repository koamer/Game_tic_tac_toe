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
#include <ncurses.h>
#include <curses.h>
#include <menu.h>

//#include "game.h"

#include "console.h"

extern bool is_running;

int main(void)
{
	srand(time(NULL));
	initscr();

	Application_info app;

	construct_application_info(&app);
	write_logs(&app, "Unfortunelly", __func__);
	
	set_atribiute(&app, 2, A_REVERSE, A_BOLD);

	addstr("I am highlighted!\n");
	getch();

	while (is_running)
	{
		//draw(field);
		is_running = false;

	}
	endwin();
	return 0;
}
