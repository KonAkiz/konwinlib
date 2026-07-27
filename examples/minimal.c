#define _POSIX_C_SOURCE 200809L

#include <stdio.h>

#define KONWINLIB_IMPLEMENTATION
#include "konwinlib.h"

#define EXIT_SUCCESS 0

#define WINDOW_TITLE "konwinlib - test"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define FPS 60

int main(void) {

	kon_init();

	kon_deinit();

	return EXIT_SUCCESS;
}
