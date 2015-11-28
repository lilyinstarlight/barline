#include <stdio.h>
#include <unistd.h>

#include "config.h"
#include "format.h"

int main(void) {
	char buf[256];
	config_t * config;

	config = config_load("barlinerc");

	if(config == NULL)
		return 1;

	while(1) {
		format(buf, sizeof(buf), config->format);
		puts(buf);
		sleep(config->interval);
	}
}
