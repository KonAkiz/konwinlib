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

#define FPS 60

int main(void) {
	if (!kon_init()) {
		fprintf(stderr, "kon_init failed!\n");
		return 1;
	}

	kon_window_t *win = kon_createWindow(WINDOW_TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, KON_WINDOW_TRANSPARENT);
	if (!win) {
		fprintf(stderr, "kon_createWindow failed!\n");
		kon_deinit();
		return 1;
	}

	kon_event_t event;
	while (!kon_windowShouldClose(win)) {
		while (pollEvent(win, &event)) {}
		kon_sleep(1.0 / FPS);
	}

	kon_destroyWindow(win);

	kon_deinit();

	return EXIT_SUCCESS;
}
