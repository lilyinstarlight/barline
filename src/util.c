#include <stdio.h>

#include "util.h"

void format_unit(char * str, size_t size, int val, const char ** units, size_t units_size) {
	float fval;
	int num;
	int idx;

	num = units_size/sizeof(const char *);
	idx = 0;

	while(fval > 896 && idx < num) {
		fval /= 1024;
		idx++;
	}

	snprintf(str, "%.1f %s", fval, units[idx]);
}
