
#pragma once

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glfont.h"

#define snooze(msec) \
	do { \
		struct timespec _; \
		_.tv_sec = (msec) / 1000; \
		_.tv_nsec = ((msec) - (msec) / 1000 * 1000) * 1000000; \
		while (nanosleep(&_, &_) == -1); \
	} while (0)

#define tbfreq(out) \
	do { out = 1000; } while (0)

#define tbcount(out) \
	do { \
		struct timeval _; \
		gettimeofday(&_, NULL); \
		out = (unsigned long long) (_.tv_sec * 1000. + _.tv_usec / 1000. + .5); \
	} while (0)

#undef min
#undef max
#undef abs
#undef near
#undef far

#define arrayCount(a) (sizeof(a) / sizeof((a)[0]))

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) < (b) ? (b) : (a))
#define abs(a) ((a) >= 0 ? (a) : -(a))
#define clamp(a,mn,mx) min(max(a, mn), mx)

#define keyDown(k) ((g_keys[0][(k) >> 5] & (1 << ((k) & 0x1f))) && !(g_keys[1][(k) >> 5] & (1 << ((k) & 0x1f))))
#define keyUp(k) (!(g_keys[0][(k) >> 5] & (1 << ((k) & 0x1f))) && (g_keys[1][(k) >> 5] & (1 << ((k) & 0x1f))))
#define keyHeld(k) (g_keys[0][(k) >> 5] & (1 << ((k) & 0x1f)))

#define buttonDown(b) ((g_buttons[0] & (1 << (b))) && !(g_buttons[1] & (1 << (b))))
#define buttonUp(b) (!(g_buttons[0] & (1 << (b))) && (g_buttons[1] & (1 << (b))))
#define buttonHeld(b) (g_buttons[0] & (1 << (b)))

#define glColorHex(c) \
	do { const unsigned k = (c); glColor4ubv((const unsigned char*) &k); } while (0)

extern int g_window;
extern int g_reshape;
extern float g_width;
extern float g_height;
extern float g_aspect;

extern unsigned g_keys[2][256 >> 5];
extern unsigned g_buttons[2];
extern int g_mousex;
extern int g_mousey;
