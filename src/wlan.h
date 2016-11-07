#ifndef WLAN_H
#define WLAN_H
#include <stddef.h>
#include <stdbool.h>

typedef struct {
	// parameters
	char iface[64];
} wlan_t;

void wlan_ssid(wlan_t * wlan, char * buf, size_t size);

void wlan_parse(const char * fmt, wlan_t * wlan);
int wlan_poll(wlan_t * wlan);
size_t wlan_format(wlan_t * wlan, char * buf, size_t size);
#endif
