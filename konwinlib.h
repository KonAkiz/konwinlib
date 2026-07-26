#ifndef KONWINLIB_H
#define KONWINLIB_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/*** definitions ***/

typedef struct kon_context kon_context_t;

bool kon_init(void);
void kon_deinit(void);

typedef struct kon_window kon_window_t;

typedef enum kon_windowFlags {
	KON_WINDOW_NONE = 0,
	KON_WINDOW_TRANSPARENT = 1 << 0,
	KON_WINDOW_NO_DECOR = 1 << 1,
	KON_WINDOW_ALWAYS_ONTOP = 1 << 2
} kon_windowFlags_t;

typedef enum kon_eventType {
	KON_EVENT_NONE = 0,
	KON_EVENT_CLOSE,
	KON_EVENT_RESIZE
} kon_eventType_t;

typedef struct kon_event {
	kon_eventType_t type;
	int width, height; /* will be used to store data for resize events */
} kon_event_t;

kon_window_t *kon_createWindow(const char *title, int x, int y, int width, int height, kon_windowFlags_t flags);
void kon_destoryWindow(kon_window_t *window);
int pollEvent(kon_window_t *window, kon_event_t *event);
void blitPixels(kon_window_t *window, const uint32_t *pixels, int width, int height);

/*** implementation ***/

static kon_context_t *kon_ctx;

#ifdef KONWINLIB_IMPLEMENTATION

#if defined(__linux__) || defined(__unix__)

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdlib.h>
#include <string.h>

struct kon_context {
	Display *display;
};

bool kon_init(void) {
	kon_context_t *ctx = malloc(sizeof(kon_context_t));
	if (!ctx) return false;
	ctx->display = XOpenDisplay(NULL);
	kon_ctx = ctx;
	return true;
}

void kon_deinit(void) {
	free(kon_ctx);
}

struct kon_window {
	Window window;
	GC gc;
	Visual *visual;
	int depth;
	Colormap colormap;
};

kon_window_t *kon_createWindow(const char *title, int x, int y, int width, int height, kon_windowFlags_t flags) {
	if (!kon_ctx) {
		fprintf(stderr, "kon_createWindow: There appears to be no kon_ctx. \nPlease, do kon_init(); before doing anything with konwinlib, thank you.\n");
		return NULL;
	}

	kon_window_t *window = malloc(sizeof(kon_window_t));
	if (!window) return NULL;

	int screen = DefaultScreen(kon_ctx->display);

	/* XCreateSimpleWindow(kon_ctx,  */

	return window;
}

/* void kon_destoryWindow(kon_window_t *window) { */

/* } */

/* TODO: implement the rest of these */

/* int pollEvent(kon_window_t *window, kon_event_t *event) { */

/* } */

/* void blitPixels(kon_window_t *window, const uint32_t *pixels, int width, int height) { */

/* } */

#endif /* end of linux / unix implementation */

#endif /* end of KONWINLIB_IMPLEMENTATION */

#endif
