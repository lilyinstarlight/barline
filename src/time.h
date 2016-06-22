#ifndef TIME_H
#define TIME_H
#include <stddef.h>

void time_current(time_t * time, char * buf, size_t size);

size_t time_format(const char * time, char * buf, size_t size);
#endif
