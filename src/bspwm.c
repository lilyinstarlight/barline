#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>

#include "bspwm.h"

struct bspwm_pid {
	int pid;
	struct bspwm_pid * next;
};

struct bspwm_pid seed = {0, NULL};
struct bspwm_pid * bspwm_pids = &seed;

int bspwm_subscribe(const char * events) {
	int bspwm[2];

	int ret;

	ret = pipe(bspwm);

	if (ret < 0)
		return -1;

	ret = fork();

	if (ret < 0)
		return -1;

	if (ret > 0) {
		// close write end of bspwm
		close(bspwm[1]);

		// add pid to list
		struct bspwm_pid * pid = bspwm_pids;
		while (pid->next != NULL)
			pid = pid->next;
		pid->next = malloc(sizeof(struct bspwm_pid));
		pid->next->pid = ret;
		pid->next->next = NULL;

		// return the read end
		return bspwm[0];
	}
	else {
		// close read end of bspwm
		close(bspwm[0]);

		// redirect stdout to the pipe
		dup2(bspwm[1], 1);

		// exec bspc
		execlp("bspc", "bspc", "subscribe", events, (char *)NULL);

		// should not run
		return -1;
	}
}

void bspwm_id(enum bspwm_domain domain, const char * name, char * buf, size_t size) {
	int bspwm[2];

	int ret;

	ret = pipe(bspwm);

	if (ret < 0) {
		buf[0] = '\0';
		return;
	}

	ret = fork();

	if (ret < 0) {
		buf[0] = '\0';
		return;
	}

	if (ret > 0) {
		size_t chars;

		// clear buffer by default
		buf[0] = '\0';

		// close write end of bspwm
		close(bspwm[1]);

		// wait for child to die
		waitpid(ret, NULL, 0);

		// read the buffer from the child
		chars = read(bspwm[0], buf, size);

		// remove newline and null terminate buffer
		buf[chars - 1] = '\0';
	}
	else {
		// close read end of bspwm
		close(bspwm[0]);

		// redirect stdout to the pipe
		dup2(bspwm[1], 1);

		const char * sel_flag, * dom_flag;
		switch(domain) {
			case NODE:
				sel_flag = "-n";
				dom_flag = "-N";
				break;
			case DESKTOP:
				sel_flag = "-d";
				dom_flag = "-D";
				break;
			case MONITOR:
				sel_flag = "-m";
				dom_flag = "-M";
				break;
			default:
				sel_flag = "-n";
				dom_flag = "-N";
				break;
		}

		// exec bspc
		execlp("bspc", "bspc", "query", sel_flag, name, dom_flag, (char *)NULL);
	}
}

int bspwm_readline(int bspwmfd, char * buf, size_t size) {
	int ret;

	struct pollfd bspwmpoll = {.fd = bspwmfd, .events = POLLIN};

	ret = poll(&bspwmpoll, 1, 0);

	char * pos = buf;
	while (ret > 0) {
		pos = buf;

		for(;;) {
			if (pos == buf + size - 1)
				break;

			ret = read(bspwmfd, pos, 1);

			if (ret < 0)
				break;

			if (*pos == '\n' || *pos == EOF)
				break;

			pos++;
		}

		if (pos != buf)
			*pos = '\0';

		ret = poll(&bspwmpoll, 1, 0);
	}

	return pos - buf;
}

int bspwm_next(char ** report) {
	char * pos = *report;

	while (*pos != ':') {
		if (*pos == '\0') {
			pos--;
			break;
		}

		pos++;
	}

	pos++;

	int chars = pos - *report;

	*report = pos;

	return chars;
}

int bspwm_monitor(char ** report, const char * monitor) {
	char * pos = *report;

	if (strlen(monitor) > 0) {
		char buf[64];

		buf[0] = '\0';

		do {
			while (*pos != 'M' && *pos != 'm') {
				bspwm_next(&pos);

				if (*pos == '\0')
					return -1;
			}

			pos++;

			sscanf(pos, "%63[^:]", buf);
		}
		while(strcmp(buf, monitor) != 0);
	}
	else {
		while (*pos != 'M') {
			bspwm_next(&pos);

			if (*pos == '\0')
				return -1;
		}
	}

	while (*pos != ':') {
		if (*pos == '\0')
			return -1;

		pos++;
	}

	pos++;

	int chars = pos - *report;

	*report = pos;

	return chars;
}

void bspwm_close() {
	// skip first pid
	struct bspwm_pid * pid = bspwm_pids->next;
	struct bspwm_pid * next = pid;

	while (pid != NULL) {
		// kill process
		kill(pid->pid, SIGTERM);

		// free pid
		next = pid->next;
		free(pid);

		// go to next pid
		pid = next;
	}
}
