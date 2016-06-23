#ifndef WIDGET_H
#define WIDGET_H
typedef struct widget {
	struct widget * prev;

	enum {
		BATT,
		CPU,
		MEM,
		TEMP,
		TIME,
		VOL,
		WIN,
		WLAN,
		WORK,
	} type;

	union {
		batt_t * batt;
		cpu_t * cpu;
		mem_t * mem;
		temp_t * temp;
		time_t * time;
		vol_t * vol;
		win_t * win;
		wlan_t * wlan;
		work_t * work;
	} data;
} widget_t;

void widget_parse(const char * widget, widget_t * widget);
size_t widget_format(const widget_t * widget, char * buf, size_t size);
#endif
