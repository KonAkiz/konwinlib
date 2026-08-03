/* SPDX-License-Identifier: MIT */
/*
 * konfont.h
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

#ifndef KONFONT_H
#define KONFONT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "konsofren.h"

#define KON_FONT_FIRST_CHAR 32
#define KON_FONT_NUM_CHARS 95

typedef struct kon_font kon_font_t;

/*** font declarations ***/

kon_font_t *kon_loadFont(const uint8_t *ttf_data, size_t size, float pixelHeight);
void kon_freeFont(kon_font_t *font);

kon_image *kon_fontGetAtlas(kon_font_t *font);

/*** text draw declarations ***/

void kon_drawText(kon_framebuffer_t *fb, kon_font_t *font, int x, int y, const char *text, uint32_t color);
void kon_measureText(kon_font_t *font, const char *text, int *outWidth, int *outHeight);

/*** implementatoin ***/

#ifdef KONFONT_IMPLEMENTATION

#include "stb_truetype.h"

struct kon_font {
	stbtt_bakedchar chardata[KON_FONT_NUM_CHARS];
	uint8_t *atlasPixels;
	int atlasSize;
	float pixelHeight;
};

/*** private helpers ***/

static bool kon_fontBakeAtlas(kon_font_t *font, const uint8_t *ttf_data, float pixelHeight) {
	int sizes[] = {256, 512, 1024, 2048};
	
	for (int i = 0; i < 4; i++) {
		int atlasSize = sizes[i];

		uint8_t *bitmap = malloc((size_t)atlasSize * (size_t)atlasSize);
		if (!bitmap) return false;

		int result = stbtt_BakeFontBitmap(ttf_data, 0, pixelHeight, bitmap, atlasSize, atlasSize, KON_FONT_FIRST_CHAR, KON_FONT_NUM_CHARS, font->chardata);

		if (result > 0) {
			font->atlasPixels = bitmap;
			font->atlasSize = atlasSize;
			return true;
		}

		free(bitmap);
	}

	return false;
}

/*** font implementation ***/

kon_font_t *kon_loadFont(const uint8_t *ttf_data, size_t size, float pixelHeight) {
	(void)size;

	if (!ttf_data || pixelHeight <= 0.0f) return NULL;

	kon_font_t *font = malloc(sizeof(kon_font_t));
	if (!font) return NULL;

	font->atlasPixels = NULL;
	font->pixelHeight = pixelHeight;

	if (!kon_fontBakeAtlas(font, ttf_data, pixelHeight)) {
		free(font);
		return NULL;
	}

	return font;
}

void kon_freeFont(kon_font_t *font) {
	if (!font) return;

	free(font->atlasPixels);
	free(font);
}

kon_image *kon_fontGetAtlas(kon_font_t *font) {
	if (!font) return NULL;

	int n = font->atlasSize * font->atlasSize;
	uint8_t *rgba = malloc((size_t)n * 4);
	if (!rgba) return NULL;

	for (int i = 0; i < n; i++) {
		uint8_t a = font->atlasPixels[i];
		rgba[i * 4 + 0] = 255;
		rgba[i * 4 + 1] = 255;
		rgba[i * 4 + 2] = 255;
		rgba[i * 4 + 3] = a;
	}

	kon_image *img = kon_loadImage(rgba, font->atlasSize, font->atlasSize, konFormatRGBA8);
	free(rgba);
	return img;
}

/*** text draw implementation ***/

void kon_drawText(kon_framebuffer_t *fb, kon_font_t *font, int x, int y, const char *text, uint32_t color) {
	if (!fb || !font || !text) return;

	uint32_t baseColor = color & 0x00FFFFFF;

	float cursorX = (float)x;
	float cursorY = (float)y;

	for (const char *p = text; *p; p++) {
		unsigned char c = (unsigned char)*p;

		if (c < KON_FONT_FIRST_CHAR || c >= KON_FONT_FIRST_CHAR + KON_FONT_NUM_CHARS) continue;

		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(font->chardata, font->atlasSize, font->atlasSize, c - KON_FONT_FIRST_CHAR, &cursorX, &cursorY, &q, 0);

		int src_x0 = (int)(q.s0 * font->atlasSize);
		int src_y0 = (int)(q.t0 * font->atlasSize);
		int glyphW = (int)(q.x1 - q.x0);
		int glyphH = (int)(q.y1 - q.y0);

		for (int gy = 0; gy < glyphH; gy++) {
			for (int gx = 0; gx < glyphW; gx++) {
				uint8_t alpha = font->atlasPixels[(src_y0 + gy) * font->atlasSize + (src_x0 + gx)];
				if (alpha == 0) continue;

				kon_putPixel(fb, (int)q.x0 + gx, (int)q.y0 + gy, ((uint32_t)alpha << 24) | baseColor);
			}
		}
	}
}

void kon_measureText(kon_font_t *font, const char *text, int *outWidth, int *outHeight) {
	if (!font || !text) return;

	float cursorX = 0.0f;
	float cursorY = 0.0f;
	float maxX = 0.0f;

	for (const char *p = text; *p; p++) {
		unsigned char c = (unsigned char)*p;

		if (c < KON_FONT_FIRST_CHAR || c >= KON_FONT_FIRST_CHAR + KON_FONT_NUM_CHARS) continue;

		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(font->chardata, font->atlasSize, font->atlasSize, c - KON_FONT_FIRST_CHAR, &cursorX, &cursorY, &q, 0);

		if (cursorX > maxX) maxX = cursorX;
	}

	if (outWidth) *outWidth = (int)maxX;
	if (outHeight) *outHeight = (int)font->pixelHeight;
}

#endif /* KONFONT_IMPLEMENTATION */

#endif /* KONFONT_H */
