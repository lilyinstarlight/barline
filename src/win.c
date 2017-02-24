#include <stdlib.h>
#include <string.h>

#include "barline.h"
#include "bspwm.h"

#include "win.h"

void win_current(win_t * win, char * buf, size_t size) {
	buf[0] = '\0';

	if (strlen(win->monitor) > 0) {
		char * monitor;

		monitor = strstr(win->bspwmline, " ");
		if (monitor != NULL) {
			monitor++;

			if (strncmp(monitor, win->monitor, strlen(win->monitor)) != 0)
				return;
		}
	}

	xcb_get_input_focus_cookie_t focus_cookie = xcb_get_input_focus(win->xcb);
	xcb_get_input_focus_reply_t * focus_reply = xcb_get_input_focus_reply(win->xcb, focus_cookie, NULL);

	if (focus_reply == NULL)
		return;

	// round size up with (size + 3)/4
	xcb_get_property_cookie_t prop_cookie = xcb_get_property(win->xcb, 0, focus_reply->focus, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, (size + 3)/4);
	xcb_get_property_reply_t * prop_reply = xcb_get_property_reply(win->xcb, prop_cookie, NULL);

	free(focus_reply);

	if (prop_reply == NULL)
		return;

	int len = xcb_get_property_value_length(prop_reply);
	char * name = xcb_get_property_value(prop_reply);

	if (strcmp(name, "0") != 0)
		snprintf(buf, size, "%.*s", len, name);

	free(prop_reply);
}

void win_parse(const char * fmt, win_t * win) {
	int ret = sscanf(fmt, "%63[^:]", win->monitor);

	if (ret <= 0)
		strncpy(win->monitor, monitor, sizeof(win->monitor));

	if (strlen(win->monitor) > 0)
		bspwm_id(MONITOR, win->monitor, win->monitor, sizeof(win->monitor));

	win->bspwmline[0] = '\0';
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

	int ret;

	ret = bspwm_readline(win->bspwmfd, win->bspwmline, sizeof(win->bspwmline));

	if (ret < 0)
		return 0;

	win_current(win, winbuf, sizeof(winbuf));

	size_t chars;

	chars = snprintf(buf, size, "%s", winbuf);

	return chars;
}
