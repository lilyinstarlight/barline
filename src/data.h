#ifndef DATA_H
#define DATA_H
#include <stddef.h>

int getcore(int cpu);
int gettemp(const char * thermal);
int getmem();
int getvol(const char * card, const char * selement);
int getwlan(const char * iface, char * ssid, size_t size);
int getbatt(const char * batt);
int gettime(char * buf, size_t size, const char * fmt, const char * tz);
#endif
