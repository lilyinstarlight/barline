#ifndef TIME_H
#define TIME_H
#include <stddef.h>

typedef struct {
	// parameters
	const char * fmt;
	const char * tz;
} temp_t;

void time_current(time_t * time, char * buf, size_t size);

void time_parse(const char * time, time_t * time);
size_t time_format(const time_t * time, char * buf, size_t size);
#endif
