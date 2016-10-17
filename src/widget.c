#include "widget.h"

void widget_parse(const char * fmt, widget_t * widget) {
	switch (fmt[0]) {
		case 'E':
			batt_parse(fmt + 1, widget->data.batt);
			return;
		case 'C':
			cpu_parse(fmt + 1, widget->data.cpu);
			return;
		case 'M':
			mem_parse(fmt + 1, widget->data.mem);
			return;
		case 'P':
			temp_parse(fmt + 1, widget->data.temp);
			return;
		case 'D':
			timedate_parse(fmt + 1, widget->data.timedate);
			return;
		case 'V':
			vol_parse(fmt + 1, widget->data.vol);
			return;
		case 'N':
			win_parse(fmt + 1, widget->data.win);
			return;
		case 'L':
			wlan_parse(fmt + 1, widget->data.wlan);
			return;
		case 'W':
			work_parse(fmt + 1, widget->data.work);
			return;
		default:
			return;
	}
}

int widget_poll(widget_t * widget) {
	switch (widget->type) {
		case BATT:
			return batt_poll(widget->data.batt);
		case CPU:
			return cpu_poll(widget->data.cpu);
		case MEM:
			return mem_poll(widget->data.mem);
		case TEMP:
			return temp_poll(widget->data.temp);
		case TIMEDATE:
			return timedate_poll(widget->data.timedate);
		case VOL:
			return vol_poll(widget->data.vol);
		case WIN:
			return win_poll(widget->data.win);
		case WLAN:
			return wlan_poll(widget->data.wlan);
		case WORK:
			return work_poll(widget->data.work);
		default:
			return -1;
	}
}

size_t widget_format(const widget_t * widget, char * buf, size_t size) {
	switch (widget->type) {
		case BATT:
			return batt_format(widget->data.batt, buf, size);
		case CPU:
			return cpu_format(widget->data.cpu, buf, size);
		case MEM:
			return mem_format(widget->data.mem, buf, size);
		case TEMP:
			return temp_format(widget->data.temp, buf, size);
		case TIMEDATE:
			return timedate_format(widget->data.timedate, buf, size);
		case VOL:
			return vol_format(widget->data.vol, buf, size);
		case WIN:
			return win_format(widget->data.win, buf, size);
		case WLAN:
			return wlan_format(widget->data.wlan, buf, size);
		case WORK:
			return work_format(widget->data.work, buf, size);
		default:
			return 0;
	}
}
