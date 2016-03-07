#include <stdio.h>
#include <string.h>

#include "data.h"

#include "format.h"

const int core_id = -1;

const char * temp_thermal = "thermal_zone0";

const char * mem_units[] = { "KiB", "MiB", "GiB" };

const char * vol_card = "default";
const char * vol_selement = "Master";
const char * vol_mutestr = "mute";

const char * wlan_iface = "wlan0";
const char * wlan_offstr = "off";

const char * batt_power = "BAT0";

const char * time_tz = "UTC";
const char * time_fmt = "%R";

int format_unit(char * buf, size_t size, int val, const char ** units, size_t units_size) {
	float fval;
	int num;
	int idx;

	fval = val;

	num = units_size/sizeof(const char *);
	idx = 0;

	while(fval > 896 && idx < num) {
		fval /= 1024;
		idx++;
	}

	return snprintf(buf, size, "%.1f %s", fval, units[idx]);
}

int format_core(char * buf, size_t size, const char * args) {
	int id = core_id;
	int core;

	int ret;

	if(*args != '\0') {
		ret = sscanf(args, "%d", &id);
		if(ret != 1) {
			fprintf(stderr, "warning: invalid core id (%s) ignored", args);
			return 0;
		}
	}

	core = get_core(id);

	return snprintf(buf, size, "%d %%", core);
}

int format_temp(char * buf, size_t size, const char * args) {
	const char * thermal = temp_thermal;
	int temp;

	if(*args != '\0')
		thermal = args;

	temp = get_temp(thermal);

	return snprintf(buf, size, "%d °C", temp);
}

int format_mem(char * buf, size_t size, const char * args) {
	(void)args;

	int mem;

	mem = get_mem();

	return format_unit(buf, size, mem, mem_units, sizeof(mem_units));
}

int format_vol(char * buf, size_t size, const char * args) {
	const char * card = vol_card, * selement = vol_selement, * mutestr = vol_mutestr;
	int vol;

	const char * arg;

	arg = args;
	if(*arg != '\0') {
		card = arg;

		arg = strchr(arg, ':');
		if(arg != NULL) {
			selement = arg + 1;

			arg = strchr(arg + 1, ':');
			if(arg != NULL)
				mutestr = arg + 1;
		}
	}

	vol = get_vol(card, selement);

	if(vol / 1000)
		return snprintf(buf, size, "%s", mutestr);
	else
		return snprintf(buf, size, "%d %%", vol);
}

int format_wlan(char * buf, size_t size, const char * args) {
	const char * iface = wlan_iface, * offstr = wlan_offstr;
	char ssid[32];
	int off;

	const char * arg;

	arg = args;
	if(*arg != '\0') {
		iface = arg;

		arg = strchr(arg, ':');
		if(arg != NULL)
			offstr = arg + 1;
	}

	off = get_wlan(iface, ssid, sizeof(ssid));

	if(off)
		return snprintf(buf, size, "%s", offstr);
	else
		return snprintf(buf, size, "%s", ssid);

}

int format_batt(char * buf, size_t size, const char * args) {
	const char * power = batt_power;
	int batt;

	if(*args != '\0')
		power = args;

	batt = get_batt(power);

	if(batt / 1000)
		return snprintf(buf, size, "^%d %%", batt - 1000);
	else
		return snprintf(buf, size, "%d %%", batt);
}

int format_time(char * buf, size_t size, const char * args) {
	const char * tz = time_tz, * fmt = time_fmt;
	char time[32];
	int bad;

	const char * arg;

	arg = args;
	if(*arg != '\0') {
		tz = arg;

		arg = strchr(arg, ':');
		if(arg != NULL)
			fmt = arg + 1;
	}

	bad = get_time(time, sizeof(time), fmt, tz);

	if(bad)
		return snprintf(buf, size, "%s", "none");
	else
		return snprintf(buf, size, "%s", time);
}

void format(char * buf, size_t size, const char * format) {
	size_t fidx;
	size_t bidx;
	size_t last;
	size_t len;
	int ret;
	char cmd;
	char args[64];

	char * end;

	len = strlen(format);

	for(bidx = 0, fidx = 0; bidx < size - 1 && fidx < len; fidx++) {
		if(format[fidx] == '%' && fidx + 1 < len && format[fidx + 1] == '{') {
			ret = sscanf(format + fidx, "%%{%c%63[^}]}", &cmd, args);

			end = strchr(format + fidx, '}');
			if(end == NULL)
				last = len - 1;
			else
				last = end - format;

			if(ret == 1) {
				args[0] = '\0';
			}
			else if(ret != 2) {
				buf[bidx] = format[fidx];
				bidx++;

				fprintf(stderr, "warning: invalid syntax in format string ignored");
				continue;
			}

			switch(cmd) {
				case 'C':
					bidx += format_core(buf + bidx, size - 1 - bidx, args);
					break;
				case 'H':
					bidx += format_temp(buf + bidx, size - 1 - bidx, args);
					break;
				case 'M':
					bidx += format_mem(buf + bidx, size - 1 - bidx, args);
					break;
				case 'V':
					bidx += format_vol(buf + bidx, size - 1 - bidx, args);
					break;
				case 'W':
					bidx += format_wlan(buf + bidx, size - 1 - bidx, args);
					break;
				case 'E':
					bidx += format_batt(buf + bidx, size - 1 - bidx, args);
					break;
				case 'D':
					bidx += format_time(buf + bidx, size - 1 - bidx, args);
					break;
				default:
					memcpy(buf + bidx, format + fidx, last - fidx + 1);
					bidx += last - fidx + 1;
					break;
			}

			fidx = last;
		}
		else {
			buf[bidx] = format[fidx];
			bidx++;
		}
	}

	buf[bidx] = '\0';
}
