#ifndef FORMAT_H
#define FORMAT_H
#include <stddef.h>

typedef struct {
	widget_t * widgets;
} format_t;

format_t * format_load(const char * fmt);
void format_poll(format_t * format, int timeout, char * buf, size_t size);
void format_free(format_t * format);
#endif
