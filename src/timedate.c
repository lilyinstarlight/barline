#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "timedate.h"

void timedate_current(timedate_t * timedate, char * buf, size_t size) {
	time_t rtime;
	struct tm * itime;

	setenv("TZ", timedate->tz, 1);

	rtime = time(NULL);
	itime = localtime(&rtime);

	strftime(buf, size, timedate->fmt, itime);

	unsetenv("TZ");
}

void timedate_parse(const char * fmt, timedate_t * timedate) {
	int ret = sscanf(fmt, "%63s:%63s", timedate->tz, timedate->fmt);

	if (ret <= 0) {
		strncpy(timedate->tz, "", sizeof(timedate->tz));
		strncpy(timedate->fmt, "%R", sizeof(timedate->fmt));
	}
	else if (ret == 1) {
		strncpy(timedate->fmt, "%R", sizeof(timedate->fmt));
	}
}

int timedate_poll(timedate_t * timedate) {
	return -1;
}

size_t timedate_format(timedate_t * timedate, char * buf, size_t size) {
	char tdbuf[64];

	timedate_current(timedate, tdbuf, sizeof(tdbuf));

	size_t chars;

	chars = snprintf(buf, size, "%s", tdbuf);

	return chars;
}
