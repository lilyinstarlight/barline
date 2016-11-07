#include <stdio.h>
#include <string.h>

#include "batt.h"

int batt_percent(batt_t * batt) {
	FILE * file;
	char path[64];

	int ret;

	long full, now;

	snprintf(path, sizeof(path), "/sys/class/power_supply/%s/energy_full", batt->name);

	file = fopen(path, "r");
	if (file == NULL)
		return - 1;

	ret = fscanf(file, "%ld", &full);

	fclose(file);

	if (ret != 1)
		return -1;

	snprintf(path, sizeof(path), "/sys/class/power_supply/%s/energy_now", batt->name);

	file = fopen(path, "r");
	if (file == NULL)
		return -1;

	ret = fscanf(file, "%ld", &now);

	fclose(file);

	if (ret != 1)
		return -1;

	return now * 100 / full;
}

bool batt_charging(batt_t * batt) {
	FILE * file;
	char path[64];

	char status[12];

	int ret;

	bool charging;

	snprintf(path, sizeof(path), "/sys/class/power_supply/%s/status", batt->name);

	file = fopen(path, "r");
	if (file == NULL)
		return false;

	ret = fscanf(file, "%11s", status);

	fclose(file);

	if (ret != 1)
		return false;

	charging = strcmp(status, "Charging") == 0;

	return charging;
}

void batt_parse(const char * fmt, batt_t * batt) {
	int ret = sscanf(fmt, "%63s:%d", batt->name, &batt->warn);

	if (ret <= 0) {
		strncpy(batt->name, "BAT0", sizeof(batt->name));
		batt->warn = 12;
	}
	else if (ret == 1) {
		batt->warn = 12;
	}
}

int batt_poll(batt_t * batt) {
	return -1;
}

size_t batt_format(batt_t * batt, char * buf, size_t size) {
	int percent = batt_percent(batt);
	bool charging = batt_charging(batt);

	size_t chars;

	if (percent < batt->warn) {
		if (charging)
			chars = snprintf(buf, size, "%%{!u}^%d %%%%{!u}", percent);
		else
			chars = snprintf(buf, size, "%%{!u}%d %%%%{!u}", percent);
	}
	else {
		if (charging)
			chars = snprintf(buf, size, "^%d %%", percent);
		else
			chars = snprintf(buf, size, "%d %%", percent);
	}

	return chars;
}
