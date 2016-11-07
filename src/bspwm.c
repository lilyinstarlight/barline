#include <string.h>
#include <unistd.h>

#include "bspwm.h"

int bspwm_subscribe(const char * events) {
	int bspwm[2];

	int ret;

	ret = pipe(bspwm);

	if (ret < 0)
		return -1;

	ret = fork();

	if (ret < 0)
		return -1;

	if (ret == 0) {
		// close write end of bspwm
		close(bspwm[1]);

		// return the read end
		return bspwm[0];
	}
	else {
		// close read end of bspwm
		close(bspwm[0]);

		// redirect stdout to the pipe
		dup2(bspwm[1], 1);

		// exec bspc
		execlp("bspc", "subscribe", events, (char *)NULL);

		// should not run
		return -1;
	}
}

int bspwm_readline(int bspwmfd, char * buf, size_t size) {
	char * pos = buf;

	do {
		if (pos == buf + size - 1)
			break;

		read(bspwmfd, pos, 1);
		pos++;
	}
	while (*pos != '\n' && *pos != EOF);

	if (pos != buf)
		*pos = '\0';

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
