#include <stdio.h>
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
