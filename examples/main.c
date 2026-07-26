#include <stdio.h>

#define KONWINLIB_IMPLEMENTATION
#include "konwinlib.h"

#define EXIT_SUCCESS 0

int main(void) {
	kon_context_t *ctx = kon_init();

	puts("Hello, World!");

	kon_deinit(ctx);

	return EXIT_SUCCESS;
}
