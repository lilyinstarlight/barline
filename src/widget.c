#include "widget.h"

void widget_text(const char * text, widget_t * widget) {
	widget_parse(text, &widget->data.text);
}

void widget_parse(const char * fmt, widget_t * widget) {
	char text[256];

	switch (fmt[0]) {
		case 'E':
			widget->type = BATT;
			batt_parse(fmt + 1, &widget->data.batt);
			return;
		case 'C':
			widget->type = CPU;
			cpu_parse(fmt + 1, &widget->data.cpu);
			return;
		case 'M':
			widget->type = MEM;
			mem_parse(fmt + 1, &widget->data.mem);
			return;
		case 'P':
			widget->type = TEMP;
			temp_parse(fmt + 1, &widget->data.temp);
			return;
		case 'D':
			widget->type = TIMEDATE;
			timedate_parse(fmt + 1, &widget->data.timedate);
			return;
		case 'V':
			widget->type = VOL;
			vol_parse(fmt + 1, &widget->data.vol);
			return;
		case 'N':
			widget->type = WIN;
			win_parse(fmt + 1, &widget->data.win);
			return;
		case 'L':
			widget->type = WLAN;
			wlan_parse(fmt + 1, &widget->data.wlan);
			return;
		case 'W':
			widget->type = WORK;
			work_parse(fmt + 1, &widget->data.work);
			return;
		default:
			snprintf(text, sizeof(text) - 1, "%%{%s}", fmt);
			widget_text(text, widget);
			return;
	}
}

int widget_poll(widget_t * widget) {
	switch (widget->type) {
		case BATT:
			return batt_poll(&widget->data.batt);
		case CPU:
			return cpu_poll(&widget->data.cpu);
		case MEM:
			return mem_poll(&widget->data.mem);
		case TEXT:
			return text_poll(&widget->data.text);
		case TEMP:
			return temp_poll(&widget->data.temp);
		case TIMEDATE:
			return timedate_poll(&widget->data.timedate);
		case VOL:
			return vol_poll(&widget->data.vol);
		case WIN:
			return win_poll(&widget->data.win);
		case WLAN:
			return wlan_poll(&widget->data.wlan);
		case WORK:
			return work_poll(&widget->data.work);
		default:
			return -1;
	}
}

size_t widget_format(const widget_t * widget, char * buf, size_t size) {
	switch (widget->type) {
		case BATT:
			return batt_format(&widget->data.batt, buf, size);
		case CPU:
			return cpu_format(&widget->data.cpu, buf, size);
		case MEM:
			return mem_format(&widget->data.mem, buf, size);
		case TEXT:
			return text_format(&widget->data.text, buf, size);
		case TEMP:
			return temp_format(&widget->data.temp, buf, size);
		case TIMEDATE:
			return timedate_format(&widget->data.timedate, buf, size);
		case VOL:
			return vol_format(&widget->data.vol, buf, size);
		case WIN:
			return win_format(&widget->data.win, buf, size);
		case WLAN:
			return wlan_format(&widget->data.wlan, buf, size);
		case WORK:
			return work_format(&widget->data.work, buf, size);
		default:
			return 0;
	}
}
