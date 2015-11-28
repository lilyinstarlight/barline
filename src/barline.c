#include <stdio.h>
#include <unistd.h>

#include "config.h"

int main(void) {
	config_t * config;

	config = config_load("barlinerc");

	if(config == NULL)
		return 1;

	for(;; sleep(config->interval)) {
		printf("%s\n", config->format);
	}
}
