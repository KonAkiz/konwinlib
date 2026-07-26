#ifndef KONWINLIB_H
#define KONWINLIB_H

#include <stdint.h>

/*** definitions ***/

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

kon_window_t *kon_createWindow(const char *title, int width, int height, kon_windowFlags_t flags);
void kon_destoryWindow(kon_window_t *window);
int pollEvent(kon_window_t *window, kon_event_t *event);
void blitPixels(kon_window_t *window, const uint32_t *pixels, int width, int height);

#endif
