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
		dup2(bspwm[1], stdout);

		// exec bspc
		execlp("bspc", "subscribe", events, (char *)NULL);
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
