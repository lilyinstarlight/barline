#include "mem.h"

int mem_total() {
	FILE * file;

	int ret;

	int total;

	file = fopen("/proc/meminfo", "r");
	if (file == NULL)
		return -1;

	ret = fscanf(file, "MemTotal: %d kB", &total);

	fclose(file);

	if (ret != 1)
		return -1;

	return total;
}

int mem_available() {
	FILE * file;

	int ret;

	int available;

	file = fopen("/proc/meminfo", "r");
	if (file == NULL)
		return -1;

	ret = fscanf(file, "MemTotal: %*d kB\nMemFree: %*d kB\nMemAvailable: %d kB", &available);

	fclose(file);

	if (ret != 1)
		return -1;

	return available;
}

int mem_used() {
	FILE * file;

	int ret;

	int total, available;

	file = fopen("/proc/meminfo", "r");
	if (file == NULL)
		return -1;

	ret = fscanf(file, "MemTotal: %d kB\nMemFree: %*d kB\nMemAvailable: %d kB", &total, &available);

	fclose(file);

	if (ret != 2)
		return -1;

	return total - available;
}

void mem_parse(const char * fmt, mem_t * mem) {
}

int mem_poll(const mem_t * mem) {
}

size_t mem_format(const mem_t * mem, char * buf, size_t size) {
}
