#include <stdio.h>
#include <string.h>

#include "data.h"
#include "util.h"

#include "format.h"

const char * mem_units[] = { "KiB", "MiB", "GiB" };

const char * vol_card = "default";
const char * vol_selement = "Master";
const char * vol_mutestr = "mute";

const char * wlan_iface = "wlan0";
const char * wlan_offstr = "off";

const char * time_tz = "UTC";
const char * time_fmt = "%R";

void format(char * buf, size_t size, const char * format) {
	size_t idx;
	size_t len;
	int ret;
	char cmd;
	char args[64];

	char * end;
	char * arg;

	//core
	int coreid;
	int core;

	//temp
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
	int batt;

	//time
	const char * tz = time_tz, * fmt = time_fmt;
	char time[32];
	int good;

	len = strlen(format);

	for(idx = 0; idx < len; idx++) {
		if(idx >= size - 1)
			break;

		if(format[idx] == '%' && idx + 1 < len && format[idx + 1] == '{') {
			ret = sscanf(format + idx, "%%{%c%63[^}]}", &cmd, args);

			end = strchr(format + idx, '}');
			if(end == NULL)
				idx = len;
			else
				idx = end - format;

			if(ret != 2) {
				fprintf(stderr, "warning: invalid syntax in format string ignored");
				continue;
			}

			switch(cmd) {
				case 'C':
					ret = sscanf(args, "%d", &coreid);
					if(ret != 1) {
						fprintf(stderr, "warning: invalid core id (%s) ignored", args);
						break;
					}

					core = getcore(coreid);
					snprintf(buf + idx, size - 1 - idx, "%d %%", core);

					break;
				case 'H':
					temp = gettemp(args);
					snprintf(buf + idx, size - 1 - idx, "%d °C", temp);

					break;
				case 'M':
					mem = getmem();
					format_unit(buf + idx, size - 1 - idx, mem, mem_units, sizeof(mem_units));

					break;
				case 'V':
					arg = args;
					if(*arg != '\0') {
						card = arg;

						arg = strchr(arg, ':') + 1;
						if(arg != NULL) {
							selement = arg;

							arg = strchr(arg, ':') + 1;
							if(arg != NULL)
								mutestr = arg;
						}
					}

					vol = getvol(card, selement);
					if(vol / 1000)
						snprintf(buf + idx, size - 1 - idx, "%s", mutestr);
					else
						snprintf(buf + idx, size - 1 - idx, "%d %%", vol % 101);

					break;
				case 'W':
					arg = args;
					if(*arg != '\0') {
						iface = arg;

						arg = strchr(arg, ':') + 1;
						if(arg != NULL)
							offstr = arg;
					}

					off = getwlan(iface, ssid, sizeof(ssid));
					if(off)
						snprintf(buf + idx, size - 1 - idx, "%s", offstr);
					else
						snprintf(buf + idx, size - 1 - idx, "%s", ssid);

					break;
				case 'E':
					batt = getbatt(args);
					if(batt / 1000)
						snprintf(buf + idx, size - 1 - idx, "^%d %%", batt % 101);
					else
						snprintf(buf + idx, size - 1 - idx, "%d %%", batt % 101);

					break;
				case 'D':
					arg = args;
					if(*arg != '\0') {
						tz = arg;

						arg = strchr(arg, ':') + 1;
						if(arg != NULL)
							fmt = arg;
					}

					good = gettime(time, sizeof(time), fmt, tz);
					if(good)
						snprintf(buf + idx, size - 1 - idx, "%s", time);
					else
						snprintf(buf + idx, size - 1 - idx, "%s", "none");

					break;
			}
		}
		else {
			buf[idx] = format[idx];
		}
	}

	buf[size - 1] = '\0';
}
