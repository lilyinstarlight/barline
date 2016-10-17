#include "bspwm.h"

#include "win.h"

void win_current(win_t * win, char * buf, size_t size) {
}

void win_parse(const char * fmt, win_t * win) {
	int ret = sscanf(fmt, "%63s", win->dsp);

	if (ret == 0)
		win->dsp = "";
}

int win_poll(win_t * win) {
	if (win->bspwmfd < 0)
		win->bspwmfd = bspwm_subscribe("node_focus");

	return win->bspwmfd;
}

size_t win_format(const win_t * win, char * buf, size_t size) {
	char winbuf[64];

	win_current(win, winbuf, sizeof(winbuf));

	snprintf(buf, size, "%s", winbuf);
}
