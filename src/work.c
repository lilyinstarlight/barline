#include <stdlib.h>

#include "work.h"

int work_count(work_t * work, char * bspwmline) {
	int count = 0;

	char ctrl;

	char * pos = bspwmline;

	pos++;

	bspwm_monitor(&pos, work->monitor);

	for (;;) {
		sscanf(pos, "%c%*63[^:]", &ctrl);

		if (ctrl == 'L')
			break;

		if (work->value == ALL) {
			count++;
		}
		else if (work->value == ACTIVE) {
			if (ctrl == 'o' || ctrl == 'O')
				count++;
		}
		else if (work->value == CURRENT) {
			if (ctrl == 'O')
				count++;
		}
		else {
			count++;
		}

		bspwm_next(&pos);
	}
}

void work_all(work_t * work, char * bspwmline, char ** vector, size_t inner, size_t size) {
	char ctrl;

	char buf[64];

	buf[0] = '\0';

	char * pos = bspwmline;

	pos++;

	bspwm_monitor(&pos, work->monitor);

	for (int idx = 0; idx < size; idx++) {
		sscanf(pos, "%c%63[^:]", &ctrl, buf);

		if (ctrl == 'L')
			break;

		snprintf(vector[idx], inner, "%s", buf);

		bspwm_next(&pos);
	}
}

void work_active(work_t * work, char * bspwmline, char ** vector, size_t inner, size_t size) {
	char ctrl;

	char buf[64];

	buf[0] = '\0';

	char * pos = bspwmline;

	pos++;

	bspwm_monitor(&pos, work->monitor);

	for (int idx = 0; idx < size;) {
		sscanf(pos, "%c%63[^:]", &ctrl, buf);

		if (ctrl == 'L')
			break;

		if (ctrl == 'O' || ctrl == 'o') {
			snprintf(vector[idx], inner, "%s", buf);
			idx++;
		}

		bspwm_next(&pos);
	}
}

void work_current(work_t * work, char * bspwmline, char * current, size_t size) {
	char ctrl;

	char buf[64];

	buf[0] = '\0';

	char * pos = bspwmline;

	pos++;

	bspwm_monitor(&pos, work->monitor);

	for (int idx = 0; idx < size;) {
		sscanf(pos, "%c%63[^:]", &ctrl, buf);

		if (ctrl == 'L')
			break;

		if (ctrl == 'O') {
			snprintf(current, size, "%s", buf);
			return;
		}

		bspwm_next(&pos);
	}
}

void work_parse(const char * fmt, work_t * work) {
	char type;

	int ret = sscanf(fmt, "%c:%63s:%63s", &type, work->separator, work->monitor);

	if (ret == 0) {
		work->value = ACTIVE;
		work->separator = "  ";
		work->monitor = monitor;
	}
	else if (ret == 1) {
		work->value = type == 'C' ? COUNT : type == 'A' ? ALL : type == 'T' ? ACTIVE : CURRENT;
		work->separator = "  ";
		work->monitor = monitor;
	}
	else if (ret == 2) {
		work->value = type == 'C' ? COUNT : type == 'A' ? ALL : type == 'T' ? ACTIVE : CURRENT;
		work->monitor = monitor;
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
	char current[64];

	char bspwmline[256];

	bspwm_readline(win->bspwmfd, bspwmline, sizeof(bspwmline));

	int count = work_count(work);

	if (work->value == COUNT) {
		snprintf(buf, size, "%d", count);
	}
	else if (work->value == ALL) {
		char ** vector = malloc(count*sizeof(current));

		work_all(work, vector, sizeof(current), count);

		size -= snprintf(buf, size, "%s", vector[0]);
		for (int work = 1; work < count; work++)
			size -= snprintf(buf, size, "%s%s", work->separator, vector[work]);

		free(vector);
	}
	else if (work->value == ACTIVE) {
		char ** vector = malloc(count*sizeof(current));

		work_active(work, vector, sizeof(current), count);

		size -= snprintf(buf, size, "%s", vector[0]);
		for (int work = 1; work < count; work++)
			size -= snprintf(buf, size, "%s%s", work->separator, vector[work]);

		free(vector);
	}
	else if (work->value == CURRENT) {
		work_current(work, current, sizeof(current));

		snprintf(buf, size, "%s", *vector);
	}
}
