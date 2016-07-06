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
}

int temp_poll(const temp_t * temp) {
}

size_t temp_format(const temp_t * temp, char * buf, size_t size) {
}
