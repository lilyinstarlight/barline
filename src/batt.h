#ifndef BATT_H
#define BATT_H
#include <stddef.h>

int batt_percent(batt_t * batt);
bool batt_charging(batt_t * batt);

size_t batt_format(const char * batt, char * buf, size_t size);
#endif
