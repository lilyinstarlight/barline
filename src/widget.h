#ifndef WIDGET_H
#define WIDGET_H
#include <stddef.h>

#include "batt.h"
#include "cpu.h"
#include "mem.h"
#include "temp.h"
#include "text.h"
#include "timedate.h"
#include "vol.h"
#include "win.h"
#include "wlan.h"
#include "work.h"

typedef struct widget {
	enum {
		BATT,
		CPU,
		MEM,
		TEMP,
		TEXT,
		TIMEDATE,
		VOL,
		WIN,
		WLAN,
		WORK,
	} type;

	union {
		batt_t batt;
		cpu_t cpu;
		mem_t mem;
		temp_t temp;
		text_t text;
		timedate_t timedate;
		vol_t vol;
		win_t win;
		wlan_t wlan;
		work_t work;
	} data;

	int fd;
} widget_t;

void widget_text(const char * text, widget_t * widget);
void widget_parse(const char * fmt, widget_t * widget);
int widget_poll(widget_t * widget);
size_t widget_format(const widget_t * widget, char * buf, size_t size);
#endif
