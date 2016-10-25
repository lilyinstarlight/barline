#include <stdlib.h>

#include "work.h"

int work_count(work_t * work, char * bspwmline) {
}

void work_all(work_t * work, char ** vector, size_t inner, size_t size) {
}

void work_active(work_t * work, char ** vector, size_t inner, size_t size) {
}

void work_current(work_t * work, char * buf, size_t size) {
}

void work_parse(const char * fmt, work_t * work) {
	char type;

	int ret = sscanf(fmt, "%c:%63s:%63s", &type, work->separator, work->monitor);

	if (ret == 0) {
		work->value = ACTIVE;
		work->separator = "  ";
		work->monitor = "";
	}
	else if (ret == 1) {
		work->value = type == 'C' ? COUNT : type == 'A' ? ALL : type == 'T' ? ACTIVE : CURRENT;
		work->separator = "  ";
		work->monitor = "";
	}
	else if (ret == 2) {
		work->value = type == 'C' ? COUNT : type == 'A' ? ALL : type == 'T' ? ACTIVE : CURRENT;
		work->monitor = "";
	}
	else if (ret == 3) {
		work->value = type == 'C' ? COUNT : type == 'A' ? ALL : type == 'T' ? ACTIVE : CURRENT;
	}

	work->bspwmfd = -1;
}

int work_poll(work_t * work) {
	if (work->bspwmfd < 0)
		work->bspwmfd = bspwm_subscribe("report");

	return work->bspwmfd;
}

size_t work_format(const work_t * work, char * buf, size_t size) {
	int count = work_count(work);

	char current[64];

	char bspwmline[256];

	bspwm_readline(win->bspwmfd, bspwmline, sizeof(bspwmline));

	if (work->value == COUNT) {
		snprintf(buf, size, "%d", count);
	}
	else if (work->value == ALL) {
		char ** vector = malloc(count*sizeof(current));

		work_all(work, vector, sizeof(current), count);

		size -= snprintf(buf, size, "%s", vector[0]);
		for (int work = 1; work < count; work++)
			size -= snprintf(buf, size, "%s%s", work->separator, vector[work]);
	}
	else if (work->value == ACTIVE) {
		char ** vector = malloc(count*sizeof(current));

		work_active(work, vector, sizeof(current), count);

		size -= snprintf(buf, size, "%s", vector[0]);
		for (int work = 1; work < count; work++)
			size -= snprintf(buf, size, "%s%s", work->separator, vector[work]);
	}
	else if (work->value == CURRENT) {
		work_current(work, current, sizeof(current));

		snprintf(buf, size, "%s", *vector);
	}

	free(vector);
}
