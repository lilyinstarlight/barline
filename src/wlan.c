#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <linux/wireless.h>

#include "wlan.h"

bool wlan_enabled(wlan_t * wlan) {
	int iwsock;
	struct iwreq iwreq;

	char ssid;

	int ret;

	bool enabled;

	iwsock = socket(AF_INET, SOCK_DGRAM, 0);

	if (iwsock < 0)
		return -2;

	strncpy(iwreq.ifr_name, wlan->iface, IFNAMSIZ);
	iwreq.u.essid.pointer = &ssid;
	iwreq.u.essid.length = 1;

	ret = ioctl(iwsock, SIOCGIWESSID, &iwreq);
	if (ret < 0)
		return false;

	enabled = iwreq.u.essid.length != 0;

	close(iwsock);

	return enabled;
}

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
	bool enabled = wlan_enabled(wlan);
	char wlanbuf[64];

	wlan_ssid(wlan, wlanbuf, sizeof(wlanbuf));

	size_t chars;

	if (enabled)
		chars = snprintf(buf, size, "%s", wlanbuf);
	else
		chars = snprintf(buf, size, "off");

	return chars;
}
