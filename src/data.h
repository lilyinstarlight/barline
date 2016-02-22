#include <stddef.h>

#define CORE_MAX 15

int get_core(int core);
int get_temp(const char * thermal);
int get_mem();
int get_vol(const char * card, const char * selement);
int get_wlan(const char * iface, char * ssid, size_t size);
int get_batt(const char * batt);
int get_time(char * buf, size_t size, const char * fmt, const char * tz);
