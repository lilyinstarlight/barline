#include <stdio.h>
#include <string.h>

#include "cpu.h"

int cpu_percent(cpu_t * cpu) {
	FILE * file;
	char ref[12];

	int ret;

	char name[12];
	unsigned long long user, userlow, sys, idle, total;

	int percent;

	file = fopen("/proc/stat", "r");
	if (file == NULL)
		return -1;

	if (cpu->idx < 0)
		snprintf(ref, sizeof(ref), "cpu");
	else
		snprintf(ref, sizeof(ref), "cpu%d", cpu->idx);

	do
		ret = fscanf(file, "%11s %llu %llu %llu %llu %*[^\n]", name, &user, &userlow, &sys, &idle);
	while (strcmp(name, ref) != 0 && ret != EOF);

	fclose(file);

	if (ret != 5)
		return -1;

	total = (user - cpu->user) + (userlow - cpu->userlow) + (sys - cpu->sys);
	percent = total * 100 / (total + idle - cpu->idle);

	cpu->user = user;
	cpu->userlow = userlow;
	cpu->sys = sys;
	cpu->idle = idle;

	return percent;
}

void cpu_parse(const char * fmt, cpu_t * cpu) {
	int ret = sscanf(fmt, "%d:%d", &cpu->idx, &cpu->warn);

	if (ret <= 0) {
		cpu->idx = -1;
		cpu->warn = 101;
	}
	else if (ret == 1) {
		cpu->warn = 101;
	}
}

int cpu_poll(cpu_t * cpu) {
	return -1;
}

size_t cpu_format(cpu_t * cpu, char * buf, size_t size) {
	int percent = cpu_percent(cpu);

	size_t chars;

	if (cpu->warn > 0 && percent >= cpu->warn)
		chars = snprintf(buf, size, "%%{!u}%d %%%%{!u}", percent);
	else
		chars = snprintf(buf, size, "%d %%", percent);

	return chars;
}
