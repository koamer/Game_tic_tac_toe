/**
 * @brief Implement of console
 * @file console.c
 * @author koamer
 * @date 2023-12-03
 * */
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "../headers/console.h"

#define _GNU_SOURCE

static void draw_menu_contex(Application_info *app, Window_size win_size);
static bool check_if_mouse_button_pressed(MEVENT *event);
static bool handle_input_event(MEVENT *event, Cordinates* cord);
static void draw_x(Field field);
static void draw_o(Field field);

void construct_application_info(Application_info *app)
{
	if(app == NULL){
		destroy_application_info(NULL);
	}

	raw();
	noecho();
	system("setterm -cursor off");

	keypad(initscr(), true);

	for (size_t i = 0; i < MAX_COLOR_SETS; i++)
	{
		app->set[i].set_number = i;
		app->set[i].foreground_color_value = 0;
		app->set[i].background_color_value = 0;
	}
	create_logs_file(app);
	getmaxyx(stdscr, app->max_size_y, app->max_size_x);

	if(app->max_size_x <= 30 || app->max_size_y <= 30) {
		write_logs(app, "Size of windows is to small.", __func__);
		destroy_application_info(app);
	}

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
		fprintf(stderr, "Error: App is not set");
		endwin();
		system("setterm -cursor on");
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

Cordinates get_mouse_click_postion(void) {
	MEVENT event;
	Cordinates cord;
	mousemask(ALL_MOUSE_EVENTS, NULL);
	while (handle_input_event(&event, &cord) == false) {}
	return cord;
}

void calculate_corners(Application_info *app, Field field[TABLE][TABLE], Window_size win_size) {
	for(uint32_t i = 0; i < TABLE; i++) {
		for(uint32_t j = 0; j < TABLE; j++) {
			for(uint32_t k = 0; k < CORNER_NUM; k++) {
				field[i][j].corner_cord[k].x = win_size.width_offset + ((win_size.width / 3) * j)  + ((k % 2) * (win_size.width / 3));
				field[i][j].corner_cord[k].y = win_size.height_offset + (win_size.height - ((win_size.height / 3) * i)) - ((k >= 2) * win_size.height / 3);
				fprintf(app->logs, "Field [%d][%d]: Corner:%d x:%d y:%d\n", i, j, k, field[i][j].corner_cord[k].x, field[i][j].corner_cord[k].y);
			}
		}
	}
	
}

void draw_field(Application_info* app, Window_size* win_size) {

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

	win_size->height = height_working_area;
	win_size->width = width_working_area;
	win_size->height_offset = height_offset;
	win_size->width_offset = width_offset;

	calculate_corners(app, app->contex.field, *win_size);

	draw_menu_contex(app, *win_size);
}
static void draw_menu_contex(Application_info* app, Window_size win_size) {
	const char common_part[] = "Turn: ";

	const size_t string_offset = sizeof(common_part);

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
	mvprintw(1, win_size.width + sizeof(char) + 1 - string_offset, "%s%c", common_part, whos_turn);
}
static bool check_if_mouse_button_pressed(MEVENT *event) {
	if(event->bstate & BUTTON1_CLICKED) {
		return true;
	}
	return false;
}

static bool handle_input_event(MEVENT *event, Cordinates* cord) {
	int key = getch();

	switch(key) {
		case KEY_MOUSE : {
			if(getmouse(event) == OK) {
				if(check_if_mouse_button_pressed(event) == true) {
					cord->x = event->x;
					cord->y = event->y;
					return true;
				}
			}
			break;
		}
	}
	flushinp();
	return false;
}

static inline bool is_in_field(uint32_t event_x, uint32_t event_y, Cordinates field_cordinates[CORNER_NUM]) {
	if(event_x >= field_cordinates[0].x && event_x <= field_cordinates[1].x) {
		if(event_y >= field_cordinates[2].y && event_y <= field_cordinates[0].y) {
			return true;
		}
	}
	return false;
}

void get_event_positon(int32_t *tile_x, int32_t *tile_y, Field field[TABLE][TABLE],
					   Cordinates cord) {
	for(uint32_t i = 0; i < TABLE; i++) {
		for(uint32_t j = 0; j < TABLE; j++) {
			if(is_in_field(cord.x, cord.y, field[i][j].corner_cord) == TRUE) {
				*tile_x = j;
				*tile_y = i;
			}
		}
	}
}

void draw_move(Field *field, CHARACTER move) {
	switch(move) {
		case GAME_CHARACTER_O : {
			draw_o(*field);
			break;
		}
		case GAME_CHARACTER_X : {
			draw_x(*field);
			break;
		}
		case GAME_CHARACTER_NS : {

			break;
		}
	}
}

static void draw_x(Field field) {
	UNUSED(field);
}
static void draw_o(Field field) {
	UNUSED(field);
}
