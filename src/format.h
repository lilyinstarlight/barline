#include <stddef.h>

int format_unit(char * buf, size_t size, int val, const char ** units, size_t units_size);

int format_core(char * buf, size_t size, const char * args);
int format_temp(char * buf, size_t size, const char * args);
int format_mem(char * buf, size_t size, const char * args);
int format_vol(char * buf, size_t size, const char * args);
int format_wlan(char * buf, size_t size, const char * args);
int format_batt(char * buf, size_t size, const char * args);
int format_time(char * buf, size_t size, const char * args);

void format(char * buf, size_t size, const char * format);
