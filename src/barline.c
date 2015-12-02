#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "format.h"

int main(void) {
	char buf[256];

	char * dir;
	char fname[256];
	config_t * config;

	dir = getenv("XDG_CONFIG_HOME");
	if(dir != NULL)
		snprintf(filename, sizeof(filename), "%s/barline/barlinerc", dir);
	else
		snprintf(filename, sizeof(filename), "%s/.config/barline/barlinerc", getenv("HOME"));

	config = config_load(fname);

	if(config == NULL)
		return 1;

	while(1) {
		format(buf, sizeof(buf), config->format);
		puts(buf);
		sleep(config->interval);
	}
}
