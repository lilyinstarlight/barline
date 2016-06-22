#ifndef VOL_H
#define VOL_H
#include <stdbool.h>
#include <stddef.h>

int vol_percent(vol_t * vol);
bool vol_mute(vol_t * vol);

size_t vol_format(const char * vol, char * buf, size_t size);
#endif
