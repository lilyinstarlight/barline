#ifndef CPU_H
#define CPU_H
#include <stddef.h>

int cpu_percent(cpu_t * cpu);

size_t cpu_format(const char * cpu, char * buf, size_t size);
#endif
