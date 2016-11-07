#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <linux/wireless.h>

#include "wlan.h"

void wlan_ssid(wlan_t * wlan, char * buf, size_t size) {
	int iwsock;
	struct iwreq iwreq;

	iwsock = socket(AF_INET, SOCK_DGRAM, 0);

	if (iwsock < 0) {
		buf[0] = '\0';
		return;
	}

	strncpy(iwreq.ifr_name, wlan->iface, IFNAMSIZ);
	iwreq.u.essid.pointer = buf;
	iwreq.u.essid.length = size - 1;

	ioctl(iwsock, SIOCGIWESSID, &iwreq);

	buf[iwreq.u.essid.length] = '\0';

	close(iwsock);
}

void wlan_parse(const char * fmt, wlan_t * wlan) {
	int ret = sscanf(fmt, "%63s", wlan->iface);

	if (ret <= 0)
		strncpy(wlan->iface, "wlan0", sizeof(wlan->iface));
}

int wlan_poll(wlan_t * wlan) {
	return -1;
}

size_t wlan_format(wlan_t * wlan, char * buf, size_t size) {
	char wlanbuf[64];

	wlan_ssid(wlan, wlanbuf, sizeof(wlanbuf));

	size_t chars;

	if (strlen(wlanbuf) > 0)
		chars = snprintf(buf, size, "%s", wlanbuf);
	else
		chars = snprintf(buf, size, "off");

	return chars;
}
