#include <stdbool.h>
#include <stdio.h>

#include "mem.h"

const char * units[] = {"B", "KiB", "MiB", "GiB"};
size_t units_len = sizeof(units)/sizeof(units[0]);

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
	char type;

	int ret = sscanf(fmt, "%c:%f", &type, &mem->warn);

	if (ret <= 0) {
		mem->value = USED;
		mem->warn = 0.85;
	}
	else if (ret == 1) {
		mem->value = type == 'T' ? TOTAL : type == 'A' ? AVAILABLE : USED;
		mem->warn = 0.85;
	}
	else {
		mem->value = type == 'T' ? TOTAL : type == 'A' ? AVAILABLE : USED;
	}
}

int mem_poll(mem_t * mem) {
	return -1;
}

size_t mem_format(mem_t * mem, char * buf, size_t size) {
	int total = mem_total();

	float value;

	if (mem->value == TOTAL)
		value = total;
	else if (mem->value == AVAILABLE)
		value = mem_available();
	else
		value = mem_used();

	float ratio = (float)value / (float)total;

	bool warn = false;

	if (mem->value == USED)
		warn = ratio > mem->warn;
	else
		warn = ratio < mem->warn;

	size_t unit = 0;

	while (value > 800 && unit < units_len) {
		value /= 1024;
		unit++;
	}

	size_t chars;

	if (warn)
		chars = snprintf(buf, size, "%%{!u}%.1f %s%%{!u}", value, units[unit]);
	else
		chars = snprintf(buf, size, "%.1f %s", value, units[unit]);

	return chars;
}
