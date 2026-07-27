#define _POSIX_C_SOURCE 200809L

#include <stdio.h>

#define KONWINLIB_IMPLEMENTATION
#include "konwinlib.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define WINDOW_TITLE "konwinlib - test"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define FPS 60

int main(void) {

	if (!kon_init()) {
		fprintf(stderr, "kon_init: error initializing...\n");
		return EXIT_FAILURE;
	}

	kon_window_t *window = kon_createWindow(WINDOW_TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!window) {
		fprintf(stderr, "kon_createWindow: error creating window...\n");
		kon_deinit();
		return EXIT_FAILURE;
	}

	kon_event_t event;
	while (!kon_windowShouldClose(window)) {
		while (kon_pollEvent(window, &event)) {}
	}

	puts("tested!");

	/*** cleanup ***/

	kon_destroyWindow(window);
	kon_deinit();

	return EXIT_SUCCESS;
}
