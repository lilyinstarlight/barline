#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"
#include "format.h"
#include "bspwm.h"

char fname[256];

char * monitor = "";
config_t * config = NULL;
format_t * format = NULL;

int run = 0;

void terminate(int signum) {
	(void)signum;

	run = 0;
}

void check(int signum) {
	(void)signum;

	for(;;) {
		pid_t pid = waitpid(-1, NULL, WNOHANG);

		if (pid <= 0)
			break;

		if (bspwm_check(pid)) {
			terminate(signum);
			break;
		}
	}
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

	char buf[4096];

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

	signal(SIGCHLD, check);

	signal(SIGUSR1, reload_config);

	while (run) {
		format_poll(format, config->poll, buf, sizeof(buf));

		if (puts(buf) < 0)
			run = 0;
	}

	bspwm_close();
	format_free(format);
	config_free(config);
}
