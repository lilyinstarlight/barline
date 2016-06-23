#ifndef WIN_H
#define WIN_H
#include <stddef.h>

typedef struct {
	// parameters
	const char * dsp;
} win_t;

int win_current(win_t * win);

void win_parse(const char * win, win_t * win);
size_t win_format(const win_t * win, char * buf, size_t size);
#endif
