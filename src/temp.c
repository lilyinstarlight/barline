#include "temp.h"

int temp_current(temp_t * temp) {
	FILE * file;
	char path[64];

	int ret;

	int therm;

	snprintf(path, sizeof(path), "/sys/class/thermal/%s/temp", temp->name);

	file = fopen(path, "r");
	if (file == NULL)
		return -1;

	ret = fscanf(file, "%d", &therm);

	fclose(file);

	if (ret != 1)
		return -1;

	return therm / 1000;
}

void temp_parse(const char * fmt, temp_t * temp) {
	int ret = sscanf(fmt, "%63s:%d", temp->name, &temp->warn);

	if (ret == 0) {
		temp->name = "thermal_zone0";
		temp->warn = -1;
	}
	else if (ret == 1) {
		temp->warn = -1;
	}
}

int temp_poll(temp_t * temp) {
	return -1;
}

size_t temp_format(const temp_t * temp, char * buf, size_t size) {
	int current = temp_current(temp);

	if (current > temp->warn)
		snprintf(buf, size, "%%{!u}%d °C%%{!u}", percent);
	else
		snprintf(buf, size, "%d °C", percent);
}
