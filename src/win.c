#include <string.h>

#include "barline.h"
#include "bspwm.h"

#include "win.h"

void win_current(win_t * win, char * bspwmline, char * buf, size_t size) {
	if (strlen(win->monitor) > 0) {
		strtok(bspwmline, " ");
		char * monitor = strtok(NULL, " ");

		if (strcmp(monitor, win->monitor) != 0) {
			buf[0] = '\0';
			return;
		}
	}

	xcb_get_input_focus_cookie_t focus_cookie = xcb_get_input_focus(win->xcb);
	xcb_get_input_focus_reply_t * focus_reply = xcb_get_input_focus_reply(win->xcb, focus_cookie, NULL);

	if (focus_reply == NULL) {
		buf[0] = '\0';
		return;
	}

	xcb_get_property_cookie_t prop_cookie = xcb_get_property(win->xcb, 0, focus_reply->focus, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, 0);
	xcb_get_property_reply_t * prop_reply = xcb_get_property_reply(win->xcb, prop_cookie, NULL);

	if (prop_reply == NULL) {
		buf[0] = '\0';
		return;
	}

	snprintf(buf, size, "%*s", xcb_get_property_value_length(prop_reply), (char *)xcb_get_property_value(prop_reply));
}

void win_parse(const char * fmt, win_t * win) {
	int ret = sscanf(fmt, "%63s", win->monitor);

	if (ret <= 0)
		strncpy(win->monitor, monitor, sizeof(win->monitor));

	win->bspwmfd = -1;
	win->xcb = NULL;
}

int win_poll(win_t * win) {
	if (win->bspwmfd < 0) {
		win->bspwmfd = bspwm_subscribe("node_focus");
		win->xcb = xcb_connect(NULL, NULL);
	}

	return win->bspwmfd;
}

size_t win_format(win_t * win, char * buf, size_t size) {
	char winbuf[64];

	char bspwmline[256];

	bspwm_readline(win->bspwmfd, bspwmline, sizeof(bspwmline));

	win_current(win, bspwmline, winbuf, sizeof(winbuf));

	size_t chars;

	chars = snprintf(buf, size, "%s", winbuf);

	return chars;
}
