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
}

int batt_poll(const batt_t * batt) {
}

size_t batt_format(const batt_t * batt, char * buf, size_t size) {
}
