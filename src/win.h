#ifndef WIN_H
#define WIN_H
#include <stddef.h>

int win_current(win_t * win);

size_t win_format(const char * win, char * buf, size_t size);
#endif
