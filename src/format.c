#include "format.h"

format_t * format_load(const char * fmt) {
	format_t * format = malloc(sizeof(format_t));

	const char * c = fmt;
	while (*c != '\0') {
		if (*c == '%') {
		}
		else {
		}
	}
}

void format_poll(format_t * format, int timeout, char * buf, size_t size) {
}

void format_free(format_t * format) {
	for (int widget = 0; widget < format->num_widgets; widget++)
		free(format->widgets[widget]);

	free(format);
}
