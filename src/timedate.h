#ifndef TIMEDATE_H
#define TIMEDATE_H
#include <stddef.h>

typedef struct {
	// parameters
	const char * fmt;
	const char * tz;
} timedate_t;

void timedate_current(timedate_t * timedate, char * buf, size_t size);

void timedate_parse(const char * fmt, timedate_t * timedate);
int timedate_poll(const timedate_t * timedate);
size_t timedate_format(const timedate_t * timedate, char * buf, size_t size);
#endif
