#include <stdio.h>

#include "format.h"

void format(char * buf, size_t size, const char * format) {
	buf[0] += size + format[0];
	buf[0] -= size + format[0];
}
