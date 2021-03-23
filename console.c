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
		fprintf(stderr, "App is not set");
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
		fprintf(stderr, "App is not set %s", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}

	if(has_colors() == false) {
		fprintf(stderr, "Error: Your system doesn't support colors in ncurser API %s ", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}

	start_color();

	if(app->set[number_of_set].background_color_value == 0 &&
		app->set[number_of_set].foreground_color_value == 0) {
			fprintf(stderr, "Error: Incorrect value in set of colors %s", __func__);
			endwin();
			exit(EXIT_FAILURE);
	}

	if(can_change_color() == false) {
		fprintf(stderr, "Warning: Your system is capability of setting color once, be careful");		
	}






	// init_pair((short)number_of_set, app->set[number_of_set].;

}

void create_logs_file(Application_info *app) {

	static bool is_done = false;
	if(is_done == false) {
		app->logs = fopen("logs","a+");
		if(app->logs == NULL) {
			fprintf(stderr, "Cannot create or open logs file ");
			exit(EXIT_FAILURE);
		}
		else {
			is_done = true;
		}		
	}
}

void write_logs(Application_info *app, const char* message) {
	time_t current_time = time(NULL);
	if(app == NULL) {
		fprintf(stderr, "App is not set %s", __func__);
		endwin();
		exit(EXIT_FAILURE);
	}

	if(app->logs == NULL) {
		fprintf(stderr, "Logs file is not initialized");
		return;
	} 

	if(current_time == ((time_t) -1)) {
		fprintf(app->logs, "Failure to obtain the current time.\n");
		return;
	}

	fseek(app->logs, 0, SEEK_END);
	fprintf(app->logs, "%s \n" , message);
}

void destroy_application_info(Application_info *app) {

	// TODO: destroy all set of colors
	if(fclose(app->logs) == EOF) {
		fprintf(stderr, "Cannot close file ");
		return;
	}
}