#define _POSIX_C_SOURCE 200809L

#include <stdio.h>

#define KONWINLIB_IMPLEMENTATION
#include "konwinlib.h"

#define KONTIME_IMPLEMENTATION
#include "kontime.h"

#define KONSOFREN_IMPLEMENTATION
#include "konsofren.h"

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

	kon_window_t *win = kon_createWindow(WINDOW_TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, KON_WINDOW_TRANSPARENT | KON_WINDOW_RESIZABLE | KON_WINDOW_CENTER);
	if (!win) {
		fprintf(stderr, "kon_createWindow failed!\n");
		kon_deinit();
		return 1;
	}
	kon_setExitKey(win, KON_KEY_Q);

	kon_framebuffer_t *fb = kon_createFramebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!fb) {
		fprintf(stderr, "kon_createFramebuffer failed!\n");
		kon_destroyWindow(win);
		kon_deinit();
	}

	uint8_t *export_buf = malloc((size_t)fb->width * fb->height * 4);
	if (!export_buf) {
		fprintf(stderr, "failed at creating export buffer... somehow...\n");
		kon_freeFramebuffer(fb);
		kon_destroyWindow(win);
		kon_deinit();
	}

	kon_setWindowPos(win, 1000, 80);

	kon_setWindowSize(win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	int x, y, width, height;

	kon_getWindowPos(win, &x, &y);

	kon_getWindowSize(win, &width, &height);

	printf("Window position is: x:%d y:%d\n", x, y);

	printf("Window size is: x:%d y:%d\n", width, height);

	kon_event_t event;
	while (!kon_windowShouldClose(win)) {
		while (kon_pollEvent(win, &event)) {
			if (event.type == KON_EVENT_RESIZE) {
				kon_resizeFramebuffer(fb, event.width, event.height);

				uint8_t *tmp = realloc(export_buf, (size_t)fb->width * fb->height * 4);
				if (tmp) export_buf = tmp;
			}
		}

		kon_clearFramebuffer(fb, 0x00000000);

		kon_fillRectangle(fb, 50, 50, 50, 50, 0xFF0000FF);

		kon_exportPixels(fb, konFormatBGRA8, export_buf);
		kon_blitPixels(win, (uint32_t *)export_buf, fb->width, fb->height);

		kon_sleep(1.0 / FPS);
	}

	/*** cleanup ***/

	free(export_buf);
	kon_freeFramebuffer(fb);
	kon_destroyWindow(win);
	kon_deinit();

	return EXIT_SUCCESS;
}
