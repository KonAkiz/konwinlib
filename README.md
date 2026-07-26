# KONWINLIB
## KonAki's Windowing Library

![License](https://img.shields.io/badge/license-MIT-blue)
![Language](https://img.shields.io/badge/language-C99-orange)

## About

This project was made for me to have a library for making and managing windows. It's not the cleanest as of now, but I will be using this for some of my other projects.

This project is a single header library that was built with [c99](https://en.wikipedia.org/wiki/C99) in mind.

It's very basic and simple as of now since it's still early in development.

### Supported Platforms:
- linux

### Planned future support:
- windows

## Usage

You don't have to compile this project. The [Makefile](Makefile) was just made for testing and making the example.

All you have to do is just drag and drop the header file into your project, and make one file define KONWINLIB_IMPLEMENTATION before including it.
```c
#define KONWINLIB_IMPLEMENTATION
#include "konwinlib.h"
```

Alternatively, you could define it during compile time.
```sh
cc -std=c99 -DKONWINLIB_IMPLEMENTATION -D_POSIX_C_SOURCE=200809L main.c -o test -lX11 -lXrandr
```
I also define _POSIX_C_SOURCE as 200809l because one of the projects used for the example needs it defined to expose some posix stuff we need.

## Examples

A small mini example to what it looks like when implemented into a project is this:
```c
#define KONWINLIB_IMPLEMENTATION
#include "konwinlib.h"

#define WINDOW_TITLE "konwinlib - example"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_X 10
#define WINDOW_Y 10

int main(void) {
	kon_init();

	kon_window_t *win = kon_createWindow(WINDOW_TITLE, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, KON_WINDOW_TRANSPARENT);

	kon_event_t event;
	while (!kon_windowShouldClose(win)) {
		while (pollEvent(win, &event)) {/* you can use this for handling events later */}
	}
	
	kon_destroyWindow(win);
	kon_deinit();
	return 0;
}
```

one with error handling is this one:
```c
#include <stdio.h>

/* only define the implementation in one file */
#define KONWINLIB_IMPLEMENTATION

/* you can include it as much as you want though... it does have a header guard */
#include "konwinlib.h"

#define WINDOW_TITLE "konwinlib - example"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_X 10
#define WINDOW_Y 10

int main(void) {
	if (!kon_init()) {
		fprintf(stderr, "kon_init: error initializing\n");
		return 1;
	}

	kon_window_t *win = kon_createWindow(WINDOW_TITLE, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, KON_WINDOW_TRANSPARENT);
	if (!win) {
		kon_deinit();
		fprintf(stderr, "kon_createWindow: error creating window\n");
		return 1;
	}

	kon_event_t event;
	while (!kon_windowShouldClose(win)) {
		while (pollEvent(win, &event)) {/* you can use this for handling events later */}
	}
	
	kon_destroyWindow(win);
	kon_deinit();
	return 0;
}
```

This thing has only been tested with a basic software renderer. [KonSofRen](https://github.com/KonAkiz/konsofren) to be exact. 

Even though it's closer to a rasterizer as of the creation of this README.md. I will still be calling it a software renderer.

My [main.c file](examples/main.c) contains an example implementation that's hooked up to a software renderer. You can use it as an example.

If you wish to see an example setup, you can check my [examples dir/folder](examples) or my [main.c file](examples/main.c) that I use for prototyping.

## Future Goals:
- add windows support

## License - [MIT License](LICENSE)
