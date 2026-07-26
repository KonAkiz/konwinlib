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
void kon_destroyWindow(kon_window_t *window);
int pollEvent(kon_window_t *window, kon_event_t *event);
void blitPixels(kon_window_t *window, const uint32_t *pixels, int width, int height);

/*** implementation ***/

#ifdef KONWINLIB_IMPLEMENTATION

static kon_context_t *kon_ctx;

#if defined(__linux__) || defined(__unix__)

#include <X11/Xlib.h>
#include <X11/Xutil.h>
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
	if (!ctx->display) return false;
	kon_ctx = ctx;
	return true;
}

void kon_deinit(void) {
	if (!kon_ctx) return;

	XCloseDisplay(kon_ctx->display);
	free(kon_ctx);
	kon_ctx=NULL;
}

struct kon_window {
	Window window;
	GC gc;
	Visual *visual;
	int depth;
	Colormap colormap;
	bool owns_colormap;
};

kon_window_t *kon_createWindow(const char *title, int x, int y, int width, int height, kon_windowFlags_t flags) {
	if (!kon_ctx) {
		fprintf(stderr, "kon_createWindow: There appears to be no kon_ctx. \nPlease, do kon_init(); before doing anything with konwinlib, thank you.\n");
		return NULL;
	}

	kon_window_t *window = malloc(sizeof(kon_window_t));
	if (!window) return NULL;

	int screen = DefaultScreen(kon_ctx->display);

	if (flags & KON_WINDOW_TRANSPARENT) {
		XVisualInfo vinfo;
		if (!XMatchVisualInfo(kon_ctx->display, screen, 32, TrueColor, &vinfo)) {
			free(window);
			return NULL;
		}

		window->visual   = vinfo.visual;
		window->depth    = vinfo.depth;
		window->colormap = XCreateColormap(kon_ctx->display, RootWindow(kon_ctx->display, screen), window->visual, AllocNone);

		XSetWindowAttributes attrs;
		attrs.colormap = window->colormap;
		attrs.background_pixel = 0;
		attrs.border_pixel = 0;

		window->window = XCreateWindow(
									   kon_ctx->display, RootWindow(kon_ctx->display, screen),
									   x, y, width, height, 0, window->depth, InputOutput,
									   window->visual, CWColormap | CWBackPixel | CWBorderPixel, &attrs);

		window->owns_colormap = true;
	} else {
		window->visual   = DefaultVisual(kon_ctx->display, screen);
		window->depth    = DefaultDepth(kon_ctx->display, screen);
		window->colormap = DefaultColormap(kon_ctx->display, screen);

		window->window = XCreateSimpleWindow(
											 kon_ctx->display, RootWindow(kon_ctx->display, screen),
											 x, y, width, height, 0, 0, 0);

		window->owns_colormap = false;
	}

	if (!window->window) {
		free(window);
		return NULL;
	}

	XStoreName(kon_ctx->display, window->window, title);
	XSelectInput(kon_ctx->display, window->window, ExposureMask | StructureNotifyMask);

	if (flags & KON_WINDOW_NO_DECOR) {
		Atom motif_hints = XInternAtom(kon_ctx->display, "_MOTIF_WM_HINTS", False);
		struct { unsigned long flags, functions, decorations; long input_mode; unsigned long status; } hints;
		memset(&hints, 0, sizeof(hints));
		hints.flags = 2;
		hints.decorations = 0;
		XChangeProperty(kon_ctx->display, window->window, motif_hints,
						motif_hints, 32, PropModeReplace, (unsigned char *)&hints, 5);
	}

	if (flags & KON_WINDOW_ALWAYS_ONTOP) {
		Atom wm_state = XInternAtom(kon_ctx->display, "_NET_WM_STATE", False);
		Atom wm_above = XInternAtom(kon_ctx->display, "_NET_WM_STATE_ABOVE", False);
		XChangeProperty(kon_ctx->display, window->window, wm_state, XA_ATOM, 32,
						PropModeReplace, (unsigned char *)&wm_above, 1);
	}

	Atom wm_delete = XInternAtom(kon_ctx->display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(kon_ctx->display, window->window, &wm_delete, 1);

	window->gc = XCreateGC(kon_ctx->display, window->window, 0, NULL);

	XMapWindow(kon_ctx->display, window->window);
	XFlush(kon_ctx->display);

	return window;
}

void kon_destroyWindow(kon_window_t *window) {
	if (!window) return;
	if (!kon_ctx) { free(window); return; }

	if (window->gc) XFreeGC(kon_ctx->display, window->gc);
	if (window->window) XDestroyWindow(kon_ctx->display, window->window);
	if (window->owns_colormap && window->colormap) XFreeColormap(kon_ctx->display, window->colormap);

	XFlush(kon_ctx->display);

	free(window);
}

/* TODO: implement the rest of these */

/* int pollEvent(kon_window_t *window, kon_event_t *event) { */

/* } */

/* void blitPixels(kon_window_t *window, const uint32_t *pixels, int width, int height) { */

/* } */

#endif /* end of linux / unix implementation */

#endif /* end of KONWINLIB_IMPLEMENTATION */

#endif
