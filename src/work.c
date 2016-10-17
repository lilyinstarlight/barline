#include <stdlib.h>

#include "work.h"

int work_count(work_t * work) {
}

void work_all(work_t * work, char ** vector, size_t inner, size_t size) {
}

void work_active(work_t * work, char ** vector, size_t inner, size_t size) {
}

void work_current(work_t * work, char * buf, size_t size) {
}

void work_parse(const char * fmt, work_t * work) {
	char type;

	int ret = sscanf(fmt, "%c:%63s", &type, work->separator);

	if (ret == 0) {
		work->value = ACTIVE;
		work->separator = "  ";
	}
	else if (ret == 1) {
		work->value = type == 'C' ? COUNT : type == 'A' ? ALL : type == 'T' ? ACTIVE : CURRENT;
		work->separator = "  ";
	}
	else if (ret == 2) {
		work->value = type == 'C' ? COUNT : type == 'A' ? ALL : type == 'T' ? ACTIVE : CURRENT;
	}
}

int work_poll(work_t * work) {
	if (work->bspwmfd < 0)
		work->bspwmfd = bspwm_subscribe("report");

	return work->bspwmfd;
}

size_t work_format(const work_t * work, char * buf, size_t size) {
	int count = work_count(work);

	char ** vector = malloc(count*64);

	if (work->value == COUNT) {
		snprintf(buf, size, "%d", count);
	}
	else if (work->value == ALL) {
		work_all(work, vector, 64, count);

		// TODO
	}
	else if (work->value == ACTIVE) {
		work_active(work, vector, 64, count);

		// TODO
	}
	else if (work->value == CURRENT) {
		work_current(work, *vector, 64);

		snprintf(buf, size, "%s", *vector);
	}

	free(vector);
}
