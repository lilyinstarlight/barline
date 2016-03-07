#include <stdio.h>
#include <string.h>

#include "data.h"
#include "util.h"

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

void format(char * buf, size_t size, const char * format) {
	size_t fidx;
	size_t bidx;
	size_t last;
	size_t len;
	int ret;
	char cmd;
	char args[64];

	char * end;
	char * arg;

	//core
	int coreid = core_id;
	int core;

	//temp
	const char * thermal = temp_thermal;
	int temp;

	//mem
	int mem;

	//vol
	const char * card = vol_card, * selement = vol_selement, * mutestr = vol_mutestr;
	int vol;

	//wlan
	const char * iface = wlan_iface, * offstr = wlan_offstr;
	char ssid[32];
	int off;

	//batt
	const char * power = batt_power;
	int batt;

	//time
	const char * tz = time_tz, * fmt = time_fmt;
	char time[32];
	int bad;

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
					if(*args != '\0') {
						ret = sscanf(args, "%d", &coreid);
						if(ret != 1) {
							fprintf(stderr, "warning: invalid core id (%s) ignored", args);
							break;
						}
					}

					core = get_core(coreid);
					bidx += snprintf(buf + bidx, size - 1 - bidx, "%d %%", core);

					break;
				case 'H':
					if(*args != '\0')
						thermal = args;

					temp = get_temp(thermal);
					bidx += snprintf(buf + bidx, size - 1 - bidx, "%d °C", temp);

					break;
				case 'M':
					mem = get_mem();
					bidx += format_unit(buf + bidx, size - 1 - bidx, mem, mem_units, sizeof(mem_units));

					break;
				case 'V':
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
						bidx += snprintf(buf + bidx, size - 1 - bidx, "%s", mutestr);
					else
						bidx += snprintf(buf + bidx, size - 1 - bidx, "%d %%", vol % 101);

					break;
				case 'W':
					arg = args;
					if(*arg != '\0') {
						iface = arg;

						arg = strchr(arg, ':');
						if(arg != NULL)
							offstr = arg + 1;
					}

					off = get_wlan(iface, ssid, sizeof(ssid));
					if(off)
						bidx += snprintf(buf + bidx, size - 1 - bidx, "%s", offstr);
					else
						bidx += snprintf(buf + bidx, size - 1 - bidx, "%s", ssid);

					break;
				case 'E':
					if(*args != '\0')
						power = args;

					batt = get_batt(power);
					if(batt / 1000)
						bidx += snprintf(buf + bidx, size - 1 - bidx, "^%d %%", batt - 1000);
					else
						bidx += snprintf(buf + bidx, size - 1 - bidx, "%d %%", batt);

					break;
				case 'D':
					arg = args;
					if(*arg != '\0') {
						tz = arg;

						arg = strchr(arg, ':');
						if(arg != NULL)
							fmt = arg + 1;
					}

					bad = get_time(time, sizeof(time), fmt, tz);
					if(bad)
						bidx += snprintf(buf + bidx, size - 1 - bidx, "%s", "none");
					else
						bidx += snprintf(buf + bidx, size - 1 - bidx, "%s", time);

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
