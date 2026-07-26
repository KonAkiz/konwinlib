/* SPDX-License-Identifier: MIT */
/*
 * kontime.h
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

#ifndef KONTIME_H
#define KONTIME_H

/*** definition ***/

typedef double kon_time;

kon_time kon_getTime(void);

void kon_sleep(kon_time seconds);

/*** implementation ***/

#ifdef KONTIME_IMPLEMENTATION

#if defined(__linux__) || defined(__unix__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)

#define _POSIX_C_SOURCE 200809L
#include <time.h>

kon_time kon_getTime(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  return (kon_time)ts.tv_sec + (kon_time)ts.tv_nsec / 1e9; 
}

void kon_sleep(kon_time seconds) {
	struct timespec ts;
	ts.tv_sec  = (time_t)seconds;
	ts.tv_nsec = (long)((seconds - ts.tv_sec) * 1e9);
		
	nanosleep(&ts, NULL);
}

#elif defined(_WIN32)

#include <windows.h>

kon_time kon_getTime(void) {
	LARGE_INTEGER freq, now;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&now);

	return (kon_time)now.QuadPart / (kon_time)freq.QuadPart;
}

void kon_sleep(kon_time seconds) {
	sleep((DWORD)(seconds * 1000.0));
}

#else
#error "kontime.h: unsupported platform!"

#endif

#endif /* end of KONTIME_IMPLEMENTATION */


#endif
