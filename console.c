/**
 * @brief Implement of console
 * @file console.c
 * @author koamer
 * @date 2022-10-20
 * */
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "console.h"

#define _GNU_SOURCE

typedef struct Window_size
{
	uint32_t width;
	uint32_t width_offset;

	uint32_t height;
	uint32_t height_offset;
} Window_size;


static void draw_menu_contex(Application_info *app, Window_size *win_size);

void construct_application_info(Application_info *app)
{
	raw();
	noecho();
	system("setterm -cursor off");
	getmaxyx(stdscr, app->max_size_y, app->max_size_x);

	mousemask(REPORT_MOUSE_POSITION, NULL);

	for (size_t i = 0; i < MAX_COLOR_SETS; i++)
	{
		app->set[i].set_number = i;
		app->set[i].foreground_color_value = 0;
		app->set[i].background_color_value = 0;
	}
	create_logs_file(app);
	for(size_t i = 0 ; i < TABLE; i++) {
		for(size_t j = 0; j < TABLE; j++) {
			construct_field(&app->contex.field[i][j]);
		}
	}
	for(size_t i = 0; i < NUMBER_OF_PLAYER; i++) { 
		construct_player(&app->contex.player[i]);
	}
	app->contex.current_player_move = &app->contex.player[0];
	if(app->contex.current_player_move == NULL) {
		write_logs(app, "Players has not been initialized", __func__);
		destroy_application_info(app);
		exit(EXIT_FAILURE);
	}
}

void create_set_of_colors(Application_info *app, uint8_t background_color, 
							uint8_t foreground_color) {
	
	if(app == NULL) {
		write_logs(app, "Error: App is not set", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}
	
	size_t i = 0;
	while(app->set[i].background_color_value != 0 ||
		  app->set[i].foreground_color_value != 0) {
		i++; // TODO: Overflow
	}
	app->set[i].background_color_value = background_color;
	app->set[i].foreground_color_value = foreground_color;
}

void set_color(Application_info *app, int16_t number_of_set) {

	if(app == NULL) {
		write_logs(app, "Error: App is not set", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}

	if(has_colors() == false) {
		write_logs(app, "Error: Your system doesn't support colors in ncurser API", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}

	start_color();

	if(app->set[number_of_set].background_color_value == 0 &&
		app->set[number_of_set].foreground_color_value == 0) {
			write_logs(app, "Error: Incorrect value in set of colors", __func__);
			endwin();
			exit(EXIT_FAILURE);
	}

	if(can_change_color() == false) {
		write_logs(app, "Warning: Your system is capability of setting color once, be careful", __func__);		
	}
	// TODO: Overflow can happen here
	init_pair(number_of_set + 1, app->set[number_of_set].foreground_color_value, app->set[number_of_set].background_color_value);
	attron(COLOR_PAIR(number_of_set + 1));
}

void create_logs_file(Application_info *app) {

	if(app == NULL) {
		write_logs(app, "Error: App is not set", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}
	app->logs = fopen("logs","w+");
	if(app->logs == NULL) {
		fprintf(stderr, "Error: Cannot create or open logs file ");
		exit(EXIT_FAILURE);		
	}
}

void write_logs(Application_info* app, const char* message, const char* func) {
	if(app->logs == NULL) {
		fprintf(stderr, "Error: Logs file is not initialized");
		return;
	}

	time_t current_time = time(NULL);

	if(current_time == ((time_t) -1)) {
		fprintf(app->logs, "%s %s", "Error: Failure to obtain the current time.", __func__);
		return;
	} 

	char* time_string = ctime(&current_time);

	if(time_string == NULL) {
		fprintf(app->logs, "%s %s", "Error: Failure to convert the current time.", __func__);
		return;
	}
	time_string[strlen(time_string) - 1] = '\0'; // Removing new line at the end of the time string

	fseek(app->logs, 0, SEEK_END);
	fprintf(app->logs, "%s %s - %s\n", time_string, message, func);
}

void destroy_application_info(Application_info *app) {
	if(app == NULL) {
		write_logs(app, "Error: App is not set", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}

	for(short i = 0; i < MAX_COLOR_SETS; i++) {
		init_pair(i, 0, 0);
		app->set[i].background_color_value = 0;
		app->set[i].foreground_color_value = 0;
	}
	endwin();

	if(fclose(app->logs) == EOF) {
		fprintf(stderr, "Error: Cannot close file");
		return;
	}
	system("setterm -cursor on");
}

void set_atribiute(Application_info* app, int32_t arguments, ...) {
	va_list arg;

	if(app == NULL) {
		write_logs(app, "Error: App is not set", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}
	if(arguments == 0) {
		write_logs(app, "Error: Numbers of arguments cannot be 0 ", __func__);
		return;
	}

	va_start(arg, arguments);

	for(int i = 0; i < arguments; i++) {
		attron(va_arg(arg, int32_t));
	}
	va_end(arg);
}

Cordinates get_mouse_click_postion(Application_info* app) {
	Cordinates cord;
	MEVENT event;
	if(getmouse(&event) == OK) {
		if(event.bstate == BUTTON1_CLICKED) {
			cord.x = event.x;
			cord.y = event.y;
		}
		else {
			cord.x = 0;
			cord.y = 0;
		}
	}
	else {
		write_logs(app, "Error: Cannot get mouse event", __func__);
		cord.x = 0;
		cord.y = 0;
	}

	return cord;
}

void draw_field(Application_info* app) {

	create_set_of_colors(app, COLOR_BLACK, COLOR_WHITE);
	set_color(app, 0);

	border(0, 0, 0, 0, 0, 0, 0, 0); // prints deafult border

	const uint32_t width_offset = app->max_size_x / 4;
	const uint32_t height_offset = app->max_size_y / 8;

	const uint32_t width_working_area = app->max_size_x - 2 * width_offset;
	const uint32_t height_working_area = app->max_size_y - 2 * height_offset;
	
	mvhline(height_offset + height_working_area / 3, width_offset, 0, width_working_area);
	mvhline(height_offset + 2 * (height_working_area / 3), width_offset, 0, width_working_area);
	mvvline(height_offset, width_offset + width_working_area / 3, 0, height_working_area);
	mvvline(height_offset, width_offset + 2 * (width_working_area / 3), 0, height_working_area);

	Window_size win_size = {
		.height = height_working_area,
		.width = width_working_area,
		.height_offset = height_offset,
		.width_offset = width_offset
	};
	draw_menu_contex(app, &win_size);
}
static void draw_menu_contex(Application_info* app, Window_size* win_size) {
	const char common_part[] = "Turn: ";

	const uint32_t string_offset = sizeof(common_part);

	char whos_turn = {0};
	switch(app->contex.current_player_move->picked) {
		case GAME_CHARACTER_X: {
			whos_turn = 'X';
			break;
		}
		case GAME_CHARACTER_O: {
			whos_turn = 'O';
			break;
		}
		case GAME_CHARACTER_NS: {
			write_logs(app, "The game didn't initialize properly", __func__);
			destroy_application_info(app);
			exit(-1);
		}
		default: {
			write_logs(app, "Something REALLY went wrong but idk what", __func__);
			destroy_application_info(app);
			exit(-1);
		}

	}
	mvprintw(1, win_size->width + sizeof(char) + 1 - string_offset, "%s%c", common_part, whos_turn);
}