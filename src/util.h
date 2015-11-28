#ifndef UTIL_H
#define UTIL_H
#include <stddef.h>

void format_unit(char * buf, size_t size, int val, const char ** units, size_t units_size);
#endif
