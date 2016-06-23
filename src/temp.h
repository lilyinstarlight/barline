#ifndef TEMP_H
#define TEMP_H
#include <stdbool.h>
#include <stddef.h>

typedef struct {
	// parameters
	const char * name;
	int warn;
} temp_t;

int temp_current(temp_t * temp);

void temp_parse(const char * temp, temp_t * temp);
int temp_poll(const temp_t * temp);
size_t temp_format(const temp_t * temp, char * buf, size_t size);
#endif
