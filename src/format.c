#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "format.h"

format_t * format_load(const char * fmt) {
	int num = 0;
	int idx = 0;

	char buf[256];
	size_t bufidx = 0;

	format_t * format = malloc(sizeof(format_t));

	const char * pos = fmt;
	while (*pos != '\0') {
		if (*pos == '%') {
			if (bufidx > 0) {
				num++;
			}

			num++;

			while(*pos != '}') {
				if (*pos == '\0') {
					pos--;
					break;
				}

				pos++;
			}

			pos++;
		}
		else {
			bufidx++;

			if (bufidx >= sizeof(buf) - 1) {
				bufidx = 0;

				num++;
			}

			pos++;
		}
	}

	format->num_widgets = num;
	format->widgets = malloc(format->num_widgets*sizeof(widget_t));

	pos = fmt;
	bufidx = 0;
	while (*pos != '\0') {
		if (*pos == '%') {
			if (bufidx > 0) {
				buf[bufidx] = '\0';
				bufidx = 0;

				widget_text(buf, &format->widgets[idx]);

				idx++;
			}

			sscanf(pos, "%%{%255[^}]}", buf);

			widget_parse(buf, &format->widgets[idx]);

			idx++;

			while(*pos != '}') {
				if (*pos == '\0') {
					pos--;
					break;
				}

				pos++;
			}

			pos++;
		}
		else {
			buf[bufidx] = *pos;
			bufidx++;

			if (bufidx >= sizeof(buf) - 1) {
				buf[bufidx] = '\0';
				bufidx = 0;

				widget_text(buf, &format->widgets[idx]);

				idx++;
			}

			pos++;
		}
	}

	return format;
}

size_t format_poll(format_t * format, int timeout, char * buf, size_t size) {
	unsigned int num_fds;
	fd_set fds;

	struct timeval tv;

	num_fds = 0;
	FD_ZERO(&fds);
	for (size_t widget = 0; widget < format->num_widgets; widget++) {
		if (format->widgets[widget].fd >= 0) {
			num_fds += 1;
			FD_SET(format->widgets[widget].fd, &fds);
		}
	}

	tv.tv_sec = timeout;
	tv.tv_usec = 0;

	select(num_fds, &fds, NULL, NULL, &tv);

	size_t chars = 0;

	for (size_t widget = 0; widget < format->num_widgets; widget++) {
		if (chars >= size - 1)
			break;

		chars += widget_format(&format->widgets[widget], buf + chars, size - chars);
	}

	buf[chars] = '\0';

	return chars;
}

void format_free(format_t * format) {
	free(format->widgets);

	free(format);
}
