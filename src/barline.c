#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "format.h"

char fname[256];
config_t * config;

int run;

void reload_config(int signum) {
	(void)signum;

	config_free(config);
	config = config_load(fname);
}

int main(void) {
	char * dir;

	char buf[256];

	setvbuf(stdout, NULL, _IOLBF, 0);

	dir = getenv("XDG_CONFIG_HOME");
	if(dir != NULL)
		snprintf(fname, sizeof(fname), "%s/barline/barlinerc", dir);
	else
		snprintf(fname, sizeof(fname), "%s/.config/barline/barlinerc", getenv("HOME"));

	config = config_load(fname);

	if(config == NULL)
		return 1;

	run = 1;

	signal(SIGUSR1, reload_config);

	while(run) {
		format(buf, sizeof(buf), config->format);
		run = puts(buf) > 0;
		sleep(config->interval);
	}

	config_free(config);
}
