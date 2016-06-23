#ifndef VOL_H
#define VOL_H
#include <stdbool.h>
#include <stddef.h>

typedef struct {
	// parameters
	const char * card;
	const char * selem;
} vol_t;

int vol_percent(vol_t * vol);
bool vol_mute(vol_t * vol);

void vol_parse(const char * vol, vol_t * vol);
int vol_poll(const vol_t * vol);
size_t vol_format(const vol_t * vol, char * buf, size_t size);
#endif
