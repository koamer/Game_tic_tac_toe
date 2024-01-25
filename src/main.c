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

#include "../headers/console.h"

#define _XOPEN_SOURCE_EXTENDED

#ifndef DEBUG
	#define DEBUG 0
#endif

int main(void)
{
	srand(time(NULL));
	setlocale(LC_ALL, "");
	
	initscr();

	Application_info app;

	construct_application_info(&app);

	bool is_running = true;

	Window_size win_size;
	while (is_running)
	{
		start_game(app.contex.player, NUMBER_OF_PLAYER);
		draw_field(&app, &win_size);
		Cordinates cord = get_mouse_click_postion();
		int32_t tile_x = -1, tile_y = -1;
		get_event_positon(&tile_x, &tile_y, app.contex.field, cord);
		if((tile_x)  == -1 || (tile_y) == -1) {
			write_logs(&app, "ERR: Event postion is not in the any tile of the game", __func__);
			#if defined(DEBUG)
				fprintf(app.logs, "DBG: Evenet coordinates: x: %d, y:%d\n", cord.x, cord.y);
				fprintf(app.logs, "DBG: Tile_x: %d, Tile_y: %d\n", tile_x, tile_y);
			#endif
			continue;
		}
		#if defined(DEBUG)
			fprintf(app.logs, "DBG: Evenet coordinates: x: %d, y:%d\n", cord.x, cord.y);
			fprintf(app.logs, "DBG: Tile_x: %d, Tile_y: %d\n", tile_x, tile_y);
		#endif
		if(make_move(app.contex.current_player_move, &app.contex.field[tile_x][tile_y]) == false) {
			continue;
		}
		draw_move(&app.contex.field[tile_x][tile_y], app.contex.current_player_move->picked);
		check_is_game_over(app.contex.field);
		//switch_player();
		is_running = false;
	}
	destroy_application_info(&app);
	endwin();
	return 0;
}
