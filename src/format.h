#ifndef FORMAT_H
#define FORMAT_H
typedef struct {
	widget_t * widgets;
} format_t;

format_t * format_load(const char * fmt);
void format_poll(format_t * format, int timeout);
void format_free(format_t * format);
#endif
