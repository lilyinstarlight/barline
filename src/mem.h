#ifndef MEM_H
#define MEM_H
#include <stddef.h>

typedef struct {
	// parameters
	enum { TOTAL, AVAILABLE, USED } value;
	float warn;
} mem_t;

int mem_total();
int mem_available();
int mem_used();

const char * units[] = {"B", "KiB", "MiB", "GiB"};
size_t units_len = sizeof(units)/sizeof(units[0]);

void mem_parse(const char * fmt, mem_t * mem);
int mem_poll(mem_t * mem);
size_t mem_format(const mem_t * mem, char * buf, size_t size);
#endif
