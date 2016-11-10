#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "format.h"

char fname[256];
char * monitor;
config_t * config;
format_t * format;

int run;

void terminate(int signum) {
	(void)signum;

	run = 0;
}

void reload_config(int signum) {
	(void)signum;

	config_t * config_new;
	format_t * format_new;

	config_new = config_load(fname);
	if (config_new == NULL) {
		return;
	}

	format_new = format_load(config_new->fmt);
	if (format_new == NULL) {
		config_free(config_new);
		return;
	}

	format_free(format);
	config_free(config);

	config = config_new;
	format = format_new;
}

int main(int argc, char * argv[]) {
	char * dir;

	char buf[256];

	if (argc != 1 && argc != 2) {
		fprintf(stderr, "Usage: %s [monitor]", argv[0]);

		return 1;
	}

	if (argc == 2)
		monitor = argv[1];
	else
		monitor = "";

	setvbuf(stdout, NULL, _IOLBF, 0);

	dir = getenv("XDG_CONFIG_HOME");
	if (dir != NULL)
		snprintf(fname, sizeof(fname), "%s/barline/barlinerc", dir);
	else
		snprintf(fname, sizeof(fname), "%s/.config/barline/barlinerc", getenv("HOME"));

	config = config_load(fname);

	if (config == NULL)
		return 1;

	format = format_load(config->fmt);

	if (format == NULL)
		return 1;

	run = 1;

	signal(SIGINT, terminate);
	signal(SIGTERM, terminate);
	signal(SIGPIPE, terminate);

	signal(SIGUSR1, reload_config);

	while (run) {
		//format_poll(format, config->poll, buf, sizeof(buf));
		//TODO
		for (size_t widget = 0; widget < format->num_widgets; widget++) {
			widget_format(&format->widgets[widget], buf, sizeof(buf));
			printf("%s", buf);
		}
		printf("\n");
		sleep(1);

		//if (puts(buf) < 0)
		//	run = 0;
	}

	format_free(format);
	config_free(config);
}
