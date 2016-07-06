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
}

int timedate_poll(const timedate_t * timedate) {
}

size_t timedate_format(const timedate_t * timedate, char * buf, size_t size) {
}
