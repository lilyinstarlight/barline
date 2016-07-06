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
}

int cpu_poll(const cpu_t * cpu) {
}

size_t cpu_format(const cpu_t * cpu, char * buf, size_t size) {
}
