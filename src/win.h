#ifndef WIN_H
#define WIN_H
#include <stddef.h>

typedef struct {
	// parameters
	char dsp[64];
	// bspwm
	int bspwmfd = -1;
} win_t;

void win_current(win_t * win, char * buf, size_t size);

void win_parse(const char * fmt, win_t * win);
int win_poll(win_t * win);
size_t win_format(const win_t * win, char * buf, size_t size);
#endif
