#ifndef WLAN_H
#define WLAN_H
#include <stddef.h>

typedef struct {
	// parameters
	char iface[64];
} wlan_t;

bool wlan_enabled(wlan_t * wlan);
void wlan_ssid(wlan_t * wlan, char * buf, size_t size);

void wlan_parse(const char * fmt, wlan_t * wlan);
int wlan_poll(const wlan_t * wlan);
size_t wlan_format(const wlan_t * wlan, char * buf, size_t size);
#endif
