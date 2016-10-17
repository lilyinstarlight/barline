#ifndef WORK_H
#define WORK_H
#include <stddef.h>

typedef struct {
	// parameters
	enum { COUNT, ALL, ACTIVE, CURRENT } value;
	char separator[64];
	// bspwm
	int bspwmfd = -1;
} work_t;

int work_count(work_t * work);
void work_all(work_t * work, const char ** vector, size_t size);
void work_active(work_t * work, const char ** vector, size_t size);
void work_current(work_t * work, const char ** vector, size_t size);

void work_parse(const char * fmt, work_t * work);
int work_poll(work_t * work);
size_t work_format(const work_t * work, char * buf, size_t size);
#endif
