#define _POSIX_C_SOURCE 200809L

#include <stdio.h>

#define KONWINLIB_IMPLEMENTATION
#include "konwinlib.h"

#define KONTIME_IMPLEMENTATION
#include "kontime.h"

#define EXIT_SUCCESS 0

#define WINDOW_TITLE "konwinlib - test"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(void) {
	kon_init();

	kon_window_t *win = kon_createWindow(WINDOW_TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	kon_event_t event;
	while (!kon_windowShouldClose(win)) {
		while (pollEvent(win, &event)) {}
		kon_sleep(1.0 / 60.0);
	}

	kon_destroyWindow(win);

	kon_deinit();

	return EXIT_SUCCESS;
}
