#ifndef MEM_H
#define MEM_H
#include <stddef.h>

int mem_total(mem_t * mem);
int mem_available(mem_t * mem);
int mem_used(mem_t * mem);

size_t mem_format(const char * mem, char * buf, size_t size);
#endif
