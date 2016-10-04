#ifndef BATT_H
#define BATT_H
#include <stdbool.h>
#include <stddef.h>

typedef struct {
	// parameters
	char name[64];
	int warn;
} batt_t;

int batt_percent(batt_t * batt);
bool batt_charging(batt_t * batt);

void batt_parse(const char * fmt, batt_t * batt);
int batt_poll(const batt_t * batt);
size_t batt_format(const batt_t * batt, char * buf, size_t size);
#endif
