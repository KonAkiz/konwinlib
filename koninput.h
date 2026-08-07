/*
MIT License

Copyright (c) 2026 KonAki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef KONINPUT_H
#define KONINPUT_H

#include "konwinlib.h"

typedef struct kon_input {
	bool down[KON_KEY_COUNT];
	bool downPrev[KON_KEY_COUNT];

	bool mouseDown[3];
	bool mouseDownPrev[3];

	int mouseX, mouseY;
} kon_input_t;

void kon_inputBeginFrame(kon_input_t *input);

void kon_inputFeedEvent(kon_input_t *input, const kon_event_t *event);

bool kon_isKeyDown(const kon_input_t *input, kon_key_t key);
bool kon_isKeyPressed(const kon_input_t *input, kon_key_t key);
bool kon_isKeyReleased(const kon_input_t *input, kon_key_t key);

bool kon_isMouseDown(const kon_input_t *input, kon_mouseButton_t button);
bool kon_isMousePressed(const kon_input_t *input, kon_mouseButton_t button);
bool kon_isMouseReleased(const kon_input_t *input, kon_mouseButton_t button);

void kon_getMousePos(const kon_input_t *input, int *x, int *y);

/*** implementation ***/

#ifdef KONINPUT_IMPLEMENTATION

void kon_inputBeginFrame(kon_input_t *input) {
	if (!input) return;

	for (int i = 0; i < KON_KEY_COUNT; i++) {
		input->downPrev[i] = input->down[i];
	}
	for (int i = 0; i < 3; i++) {
		input->mouseDownPrev[i] = input->mouseDown[i];
	}
}

void kon_inputFeedEvent(kon_input_t *input, const kon_event_t *event) {
	if (!input || !event) return;

	switch (event->type) {
	case KON_EVENT_KEY_DOWN:
		if (event->key >= 0 && event->key < KON_KEY_COUNT)
			input->down[event->key] = true;
		break;
	case KON_EVENT_KEY_UP:
		if (event->key >= 0 && event->key < KON_KEY_COUNT)
			input->down[event->key] = false;
		break;
	default:
		break;
	}
}

#endif /* KONINPUT_IMPLEMENTATION */

#endif /* KONINPUT_H */
