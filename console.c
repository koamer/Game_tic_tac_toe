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
}
void create_set_of_colors(Application_info *app, uint8_t background_color, 
							uint8_t foreground_color) {
	
	if(app == NULL) {
		write_logs(app, "Error: App is not set", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}
	
	unsigned int i = 0;
	while(app->set[i].background_color_value != 0 ||
		  app->set[i].foreground_color_value != 0) {
		i++;
	}
	app->set[i].background_color_value = background_color;
	app->set[i].foreground_color_value = foreground_color;
}

void set_color(Application_info *app, uint8_t number_of_set) {

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

	// TODO : IDK
	// init_pair((short)number_of_set, app->set[number_of_set].;

}

void create_logs_file(Application_info *app) {

	if(app == NULL) {
		write_logs(&app, "Error: App is not set", __func__);
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
		write_logs(&app, "Error: App is not set", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}

	for(unsigned short i = 0; i < MAX_COLOR_SETS; i++) {
		init_pair(i, 0, 0);
		app->set[i].background_color_value = 0;
		app->set[i].foreground_color_value = 0;
	}

	if(fclose(app->logs) == EOF) {
		fprintf(stderr, "Error: Cannot close file");
		return;
	}
}