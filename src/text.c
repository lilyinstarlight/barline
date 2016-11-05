#include "text.h"

void text_parse(const char * fmt, text_t * text) {
	int ret = sscanf(fmt, "%255s", text->text);

	if (ret == 0)
		text->text = "";
}

int text_poll(text_t * text) {
	return -1;
}

size_t text_format(const text_t * text, char * buf, size_t size) {
	snprintf(buf, size, "%s", text->text);
}
