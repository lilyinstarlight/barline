#ifndef CPU_H
#define CPU_H
#include <stddef.h>

typedef struct {
	// parameters
	int idx;
	int warn;

	// states
	unsigned long long user, userlow, sys, idle, total;
} cpu_t;

int cpu_percent(cpu_t * cpu);

void cpu_parse(const char * fmt, cpu_t * cpu);
int cpu_poll(cpu_t * cpu);
size_t cpu_format(const cpu_t * cpu, char * buf, size_t size);
#endif
