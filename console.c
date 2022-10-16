/**
 * @brief Implement of console
 * @file console.c
 * @author koamer
 * @date 2020-09-02
 * */
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "console.h"

void construct_application_info(Application_info *app)
{
	getmaxyx(stdscr, app->max_size_y, app->max_size_x);
	app->current_x = 0;
	app->current_y = 0;

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
		fprintf(stderr, "%s", "Warning: Your system is capability of setting color once, be careful");
		write_logs(app, "Warning: Your system is capability of setting color once, be careful", __func__);		
	}
	init_pair(number_of_set, app->set[number_of_set].foreground_color_value, app->set[number_of_set].background_color_value);
	attron(COLOR_PAIR(number_of_set));
}

void create_logs_file(Application_info *app) {

	if(app == NULL) {
		write_logs(app, "Error: App is not set", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}

	static bool is_done = false;
	if(is_done == false) {
		app->logs = fopen("logs","a+");
		if(app->logs == NULL) {
			fprintf(stderr, "Error: Cannot create or open logs file ");
			exit(EXIT_FAILURE);
		}
		else {
			is_done = true;
		}		
	}
}

void write_logs(Application_info *app, const char* message, const char* func) {
	if(app->logs == NULL) {
		fprintf(stderr, "Error: Logs file is not initialized");
		return;
	}

	time_t current_time = time(NULL);

	if(current_time == ((time_t) -1)) {
		fprintf(app->logs, "%s %s", "Error: Failure to obtain the current time.", __func__);
		return;
	} 

	const char* time_string = ctime(&current_time);

	if(time_string == NULL) {
		fprintf(app->logs, "%s %s", "Error: Failure to convert the current time.", __func__);
		return;
	}

	fseek(app->logs, 0, SEEK_END);
	fprintf(app->logs, "%s : %s - %s", message, func, time_string);
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
}
void set_atribiute(Application_info *app, int32_t arguments, ...) {
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
Cordinates get_mouse_click_postion(Application_info *app) {
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
void draw_field(Application_info *app, uint32_t width, uint32_t height) {
	printw("%d %d", width, height);
	mvprintw(height / 2 , width / 2, "Hello");

	border("\u2551", "\u2551", "\u2551", "\u2551", "\u2551", "\u2551", "\u2551", "\u2551");
	//box_set(stdscr, (const cchar_t *) "\u2551")
	// box(stdscr, width, height);
	//chtype type;
	// 			addstr("\u2551");
	// 			addstr("\u2550");
	
}