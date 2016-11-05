#ifndef TEXT_H
#define TEXT_H

typedef struct {
	// parameters
	char text[256];
} text_t;

void text_parse(const char * fmt, text_t * text);
int text_poll(text_t * text);
size_t text_format(const text_t * text, char * buf, size_t size);
#endif
