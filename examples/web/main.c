#define _POSIX_C_SOURCE 200809L

#include <stdio.h>

#define KONWINLIB_IMPLEMENTATION
#include "konwinlib.h"

#define KONSOFREN_IMPLEMENTATION
#include "konsofren.h"

#define KONTIME_IMPLEMENTATION
#include "kontime.h"

#define PLAYER_START_X 100
#define PLAYER_START_Y 100
#define PLAYER_WIDTH  50
#define PLAYER_HEIGHT 50

typedef struct {
	kon_window_t *window;
	kon_framebuffer_t *fb;
	kon_time lastTime;
	int playerX, playerY;
} game_data_t;

static void frame(void *userdata) {
	game_data_t *data = (game_data_t *)userdata;

	kon_event_t ev;
	while (kon_pollEvent(data->window, &ev)) {
		(void)ev;
	}

	kon_time currentTime = kon_getTime();
	float deltaTime = (float)(currentTime - data->lastTime);
	data->lastTime = currentTime;

	kon_clearFramebuffer(data->fb, KON_BACKGROUND_COLOR);

	kon_fillRectangle(data->fb, data->playerX, data->playerY, PLAYER_WIDTH, PLAYER_HEIGHT, 0xFFFFFFFF);

	kon_blitPixels(data->window, data->fb->data, data->fb->width, data->fb->height);
}

int main(void) {
	if (!kon_init()) {
		fprintf(stderr, "failed to init konwinlib\n");
		return 1;
	}

	kon_window_t *window = kon_createWindow("web example", 100, 100, 640, 480, KON_WINDOW_NONE);
	if (!window) {
		fprintf(stderr, "failed to create window\n");
		kon_deinit();
		return 1;
	}

	game_data_t data;
	data.window = window;
	data.fb = kon_createFramebuffer(640, 480);
	data.lastTime = kon_getTime();
	data.playerX = PLAYER_START_X;
	data.playerY = PLAYER_START_Y;

	if (!data.fb) {
		fprintf(stderr, "failed to create framebuffer\n");
		kon_destroyWindow(window);
		kon_deinit();
		return 1;
	}

	kon_runMainLoop(window, frame, &data);

	kon_freeFramebuffer(data.fb);
	kon_destroyWindow(window);
	kon_deinit();

	return 0;
}
