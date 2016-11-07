#include <stdio.h>
#include <string.h>

#include "text.h"

void text_parse(const char * fmt, text_t * text) {
	strncpy(text->text, fmt, sizeof(text->text));
}

int text_poll(text_t * text) {
	return -1;
}

size_t text_format(text_t * text, char * buf, size_t size) {
	size_t chars;

	chars = snprintf(buf, size, "%s", text->text);

	return chars;
}
