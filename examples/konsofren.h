/* SPDX-License-Identifier: MIT */
/*
 * konsofren.h
 *
 * Copyright (c) 2026 KonAki
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef KONSOFREN_H
#define KONSOFREN_H

#include <stdint.h>
#include <stdlib.h>

#define KON_BACKGROUND_COLOR 0x05050AFF

typedef struct kon_framebuffer {
	int width, height;
	uint32_t *data;
} kon_framebuffer_t;

typedef kon_framebuffer_t kon_image;

typedef enum kon_imageFormat {
	konFormatRGBA8 = 0,
	konFormatABGR8,
	konFormatARGB8,
	konFormatBGRA8
} kon_imageFormat_t;

/*** framebuffer declarations ***/

kon_framebuffer_t *kon_createFramebuffer(int width, int height);
void kon_freeFramebuffer(kon_framebuffer_t *fb);
void kon_putPixel(kon_framebuffer_t *fb, int x, int y, uint32_t color);
void kon_clearFramebuffer(kon_framebuffer_t *fb, uint32_t color);
void kon_resizeFramebuffer(kon_framebuffer_t *fb, int width, int height);
void kon_exportPixels(kon_framebuffer_t *fb, kon_imageFormat_t format, uint8_t *out);

/*** image declarations ***/

kon_image *kon_loadImage(const uint8_t *pixels, int width, int height, kon_imageFormat_t format);
void kon_freeImage(kon_image *image);

void kon_drawImage(kon_framebuffer_t *fb, int x, int y, int width, int height, kon_image *image);

/*** draw function declarations ***/

void kon_drawRectangle(kon_framebuffer_t *fb, int x, int y, int width, int height, uint32_t color);
void kon_fillRectangle(kon_framebuffer_t *fb, int x, int y, int width, int height, uint32_t color);
void kon_drawLine(kon_framebuffer_t *fb, int x0, int y0, int x1, int y1, uint32_t color);
void kon_drawCircle(kon_framebuffer_t *fb, int center_x, int center_y, int radius, uint32_t color);
void kon_fillCircle(kon_framebuffer_t *fb, int center_x, int center_y, int radius, uint32_t color);

/*** implementation ***/

#ifdef KONSOFREN_IMPLEMENTATION

/*** private helper ***/

static inline uint32_t kon_blendColor(uint32_t dst, uint32_t src) {
	
	uint8_t src_a = (src >> 0)  & 0xFF;
	uint8_t src_b = (src >> 8)  & 0xFF;
	uint8_t src_g = (src >> 16) & 0xFF;
	uint8_t src_r = (src >> 24) & 0xFF;

	if (src_a == 0xFF) {
		return src;
	}

	uint8_t dst_a = (dst >> 0)  & 0xFF;
	uint8_t dst_b = (dst >> 8)  & 0xFF;
	uint8_t dst_g = (dst >> 16) & 0xFF;
	uint8_t dst_r = (dst >> 24) & 0xFF;

	uint8_t inv_a = 255 - src_a;

	uint8_t out_r = (src_r * src_a + dst_r * inv_a) / 255;
	uint8_t out_g = (src_g * src_a + dst_g * inv_a) / 255;
	uint8_t out_b = (src_b * src_a + dst_b * inv_a) / 255;

	return ((uint32_t)out_r << 24) | ((uint32_t)out_g << 16) | ((uint32_t)out_b << 8) | (uint32_t)dst_a;
}

/*** framebuffer implementation ***/

kon_framebuffer_t *kon_createFramebuffer(int width, int height) {
	kon_framebuffer_t *fb = malloc(sizeof(kon_framebuffer_t));
	if (!fb) return NULL;

	fb->data = malloc(width * height * sizeof(uint32_t));
	if (!fb->data) {
		free(fb);
		return NULL;
	}

	fb->width = width;
	fb->height = height;
	return fb;
}

void kon_freeFramebuffer(kon_framebuffer_t *fb) {
	if (!fb) return;

	free(fb->data);
	free(fb);
}

void kon_putPixel(kon_framebuffer_t *fb, int x, int y, uint32_t color) {
	if (!fb) return;

	if (x < 0 || x >= fb->width || y < 0 || y >= fb->height) return;

	uint32_t dst = fb->data[y * fb->width + x];
	fb->data[y * fb->width + x] = kon_blendColor(dst, color);
}

void kon_clearFramebuffer(kon_framebuffer_t *fb, uint32_t color) {
	if (!fb) return;

	for (int i = 0; i < fb->width * fb->height; i++) {
		/* did it directly to not check overhead because of the if in bounds check */
		fb->data[i] = color;
	}
}

void kon_resizeFramebuffer(kon_framebuffer_t *fb, int width, int height) {
	if (!fb) return;

	uint32_t *tmp = realloc(fb->data, (size_t)width * height * sizeof(uint32_t));
	if (!tmp) return;

	fb->data = tmp;
	fb->width  = width;
	fb->height = height;
}

void kon_exportPixels(kon_framebuffer_t *fb, kon_imageFormat_t format, uint8_t *out) {
	if (!fb || !out) return;

	int fbSize = fb->width * fb->height;

	for (int i = 0; i < fbSize; i++) {
		uint32_t color = fb->data[i];

		uint8_t a = (color >> 0)  & 0xFF;
		uint8_t b = (color >> 8)  & 0xFF;
		uint8_t g = (color >> 16) & 0xFF;
		uint8_t r = (color >> 24) & 0xFF;

		switch (format) {
		case konFormatRGBA8:
			out[i * 4 + 0] = r;
			out[i * 4 + 1] = g;
			out[i * 4 + 2] = b;
			out[i * 4 + 3] = a;
			break;
		case konFormatABGR8:
			out[i * 4 + 0] = a;
			out[i * 4 + 1] = b;
			out[i * 4 + 2] = g;
			out[i * 4 + 3] = r;
			break;
		case konFormatARGB8:
			out[i * 4 + 0] = a;
			out[i * 4 + 1] = r;
			out[i * 4 + 2] = g;
			out[i * 4 + 3] = b;
			break;
		case konFormatBGRA8:
			out[i * 4 + 0] = b;
			out[i * 4 + 1] = g;
			out[i * 4 + 2] = r;
			out[i * 4 + 3] = a;
			break;
		default:
			return;
		}
	}
}

/*** draw functions implementation ***/

void kon_drawRectangle(kon_framebuffer_t *fb, int x, int y, int width, int height, uint32_t color) {
	if (!fb) return;

	kon_drawLine(fb, x, y, x + width, y, color);
	kon_drawLine(fb, x, y + height, x + width, y + height, color);
	kon_drawLine(fb, x, y, x, y + height, color);
	kon_drawLine(fb, x + width, y, x + width, y + height, color);
}

void kon_fillRectangle(kon_framebuffer_t *fb, int x, int y, int width, int height, uint32_t color) {
	if (!fb) return;

	if (width < 0 || height < 0) return;

	if (x < 0) {
		width += x;
		x = 0;
	}
	if (y < 0) {
		height += y;
		y = 0;
	}

	if (x + width  > fb->width ) width  = fb->width  - x;
	if (y + height > fb->height) height = fb->height - y;

	for (int offset_y = 0; offset_y < height; offset_y++) {
		for (int offset_x = 0; offset_x < width; offset_x++) {
			uint32_t dst = fb->data[(offset_y + y) * fb->width + (offset_x + x)];
			fb->data[(y + offset_y) * fb->width + (x + offset_x)] = kon_blendColor(dst, color);

		}
	}
}

void kon_drawLine(kon_framebuffer_t *fb, int x0, int y0, int x1, int y1, uint32_t color) {
	if (!fb) return;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	
	int err = dx - dy;

	for (;;) {
		kon_putPixel(fb, x0, y0, color);

		if (x0 == x1 && y0 == y1) break;

		int e2 = 2 * err;

		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}

		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void kon_drawCircle(kon_framebuffer_t *fb, int center_x, int center_y, int radius, uint32_t color) {
	if (!fb) return;
	if (radius <= 0) return;

	int x = radius;
	int y = 0;
	int err = 1 - radius;

	while (x >= y) {
		kon_putPixel(fb, center_x + x, center_y + y, color);
		kon_putPixel(fb, center_x + y, center_y + x, color);
		kon_putPixel(fb, center_x - y, center_y + x, color);
		kon_putPixel(fb, center_x - x, center_y + y, color);
		kon_putPixel(fb, center_x - x, center_y - y, color);
		kon_putPixel(fb, center_x - y, center_y - x, color);
		kon_putPixel(fb, center_x + y, center_y - x, color);
		kon_putPixel(fb, center_x + x, center_y - y, color);

		y += 1;

		if (err < 0) {
			err += 2 * y + 1;
		} else {
			x -= 1;
			err += 2 * (y - x) + 1;
		}
	}
}

void kon_fillCircle(kon_framebuffer_t *fb, int center_x, int center_y, int radius, uint32_t color) {
	if (!fb) return;
	if (radius <= 0) return;

	int x = radius;
	int y = 0;
	int err = 1 - radius;

	while (x >= y) {
		kon_drawLine(fb, center_x - x, center_y + y, center_x + x, center_y + y, color);
		kon_drawLine(fb, center_x - x, center_y - y, center_x + x, center_y - y, color);
		kon_drawLine(fb, center_x - y, center_y + x, center_x + y, center_y + x, color);
		kon_drawLine(fb, center_x - y, center_y - x, center_x + y, center_y - x, color);

		y += 1;

		if (err < 0) {
			err += 2 * y + 1;
		} else {
			x -= 1;
			err += 2 * (y - x) + 1;
		}
	}
}

/*** image implementation ***/

kon_image *kon_loadImage(const uint8_t *pixels, int width, int height, kon_imageFormat_t format) {
	kon_image *image = malloc(sizeof(kon_image));
	if (!image) return NULL;

	image->data = malloc(width * height * sizeof(uint32_t));
	if (!image->data) {
		free(image);
		return NULL;
	}

	image->width = width;
	image->height = height;

	for (int i = 0; i < width * height; i++) {
		uint8_t r, g, b, a;

		switch(format) {
		case konFormatRGBA8:
			r = pixels[i * 4 + 0];
			g = pixels[i * 4 + 1];
			b = pixels[i * 4 + 2];
			a = pixels[i * 4 + 3];
			break;
		case konFormatABGR8:
			a = pixels[i * 4 + 0];
			b = pixels[i * 4 + 1];
			g = pixels[i * 4 + 2];
			r = pixels[i * 4 + 3];
			break;
		case konFormatARGB8:
			a = pixels[i * 4 + 0];
			r = pixels[i * 4 + 1];
			g = pixels[i * 4 + 2];
			b = pixels[i * 4 + 3];
			break;
		case konFormatBGRA8:
			b = pixels[i * 4 + 0];
			g = pixels[i * 4 + 1];
			r = pixels[i * 4 + 2];
			a = pixels[i * 4 + 3];
			break;
		default:
			free(image->data);
			free(image);
			return NULL;
		}

		image->data[i] = ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
	}

	return image;
}
void kon_freeImage(kon_image *image) {
	kon_freeFramebuffer(image);
}

void kon_drawImage(kon_framebuffer_t *fb, int x, int y, int width, int height, kon_image *image) {
	if (!fb || !image) return;

	/* just trying to not draw when it's unnecessary */
	if (image->width <= 0 || image->height <= 0) return;
	if (width <= 0 || height <= 0) return;
	if (x > fb->width || y > fb->height) return;

	/* using this loop to make sure we only the parts of the image that are on screen */
	for (int iy = 0; iy < height; iy++) {
		for (int ix = 0; ix < width; ix++) {
			int src_x = ix * image->width  / width;
			int src_y = iy * image->height / height;

			uint32_t color = image->data[src_y * image->width + src_x];
			kon_putPixel(fb, x+ix, y+iy, color);
		}
	}
}

#endif
#endif
