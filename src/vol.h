#ifndef VOL_H
#define VOL_H
#include <stdbool.h>
#include <stddef.h>

typedef struct {
	// parameters
	char card[64];
	char selem[64];
} vol_t;

int vol_percent(vol_t * vol);
bool vol_mute(vol_t * vol);

void vol_parse(const char * fmt, vol_t * vol);
int vol_poll(vol_t * vol);
size_t vol_format(const vol_t * vol, char * buf, size_t size);
#endif
