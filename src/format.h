#ifndef FORMAT_H
#define FORMAT_H
#include <stddef.h>

#include "widget.h"

typedef struct {
	unsigned int num_widgets;
	widget_t * widgets;
} format_t;

format_t * format_load(const char * fmt);
size_t format_poll(format_t * format, int timeout, char * buf, size_t size);
void format_free(format_t * format);
#endif
