#ifndef TEMP_H
#define TEMP_H
#include <stdbool.h>
#include <stddef.h>

int temp_current(temp_t * temp);

size_t temp_format(const char * temp, char * buf, size_t size);
#endif
