#ifndef MEM_H
#define MEM_H
#include <stddef.h>

typedef struct {
	// parameters
	enum { TOTAL, AVAILABLE, USED } value;
} mem_t;

int mem_total();
int mem_available();
int mem_used();

void mem_parse(const char * fmt, mem_t * mem);
int mem_poll(const mem_t * mem);
size_t mem_format(const mem_t * mem, char * buf, size_t size);
#endif
