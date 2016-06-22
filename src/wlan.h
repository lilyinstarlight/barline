#ifndef WLAN_H
#define WLAN_H
#include <stddef.h>

bool win_enabled(wlan_t * wlan);
void win_ssid(wlan_t * wlan, char * buf, size_t size);

size_t wlan_format(const char * wlan, char * buf, size_t size);
#endif
