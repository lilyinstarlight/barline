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

	int ret;

	iwsock = socket(AF_INET, SOCK_DGRAM, 0);

	if (iwsock < 0)
		return -2;

	strncpy(iwreq.ifr_name, wlan->iface, IFNAMSIZ);
	iwreq.u.essid.pointer = buf;
	iwreq.u.essid.length = size - 1;

	ret = ioctl(iwsock, SIOCGIWESSID, &iwreq);

	buf[iwreq.u.essid.length] = '\0';

	close(iwsock);
}

void wlan_parse(const char * fmt, wlan_t * wlan) {
	int ret = sscanf(fmt, "%63s", wlan->iface);

	if (ret == 0)
		batt->iface = "wlan0";
}

int wlan_poll(const wlan_t * wlan) {
	return -1;
}

size_t wlan_format(const wlan_t * wlan, char * buf, size_t size) {
	bool enabled = wlan_enabled(wlan);
	char wlanbuf[64];

	wlan_ssid(wlan, wlanbuf, sizeof(wlanbuf));

	if (enabled)
		snprintf(buf, size, "%s", wlanbuf);
	else
		snprintf(buf, size, "off");
}
