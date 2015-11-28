#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

config_t * config_load(const char * filename) {
	FILE * file;
	int line;
	int ret;

	char option[16];
	char buf[256];

	config_t * config;

	config = malloc(sizeof(config_t));
	memset(config, 0, sizeof(config_t));

	file = fopen(filename, "r");

	line = 0;
	while(!feof(file)) {
		ret = fscanf(file, "%15s %255[^\n]", option, buf);

		line++;

		if(ret != 2) {
			fscanf(file, "%*[^\n]");
			fprintf(stderr, "warning: invalid syntax (line %d) ignored\n", line);
			continue;
		}

		switch(option[0]) {
			case 'f':
				config->format = malloc(strlen(buf));
				strcpy(config->format, buf);
				break;
			default:
				fprintf(stderr, "warning: unknown configuration option (%s) ignored\n", option);
				break;
		}
	}

	return config;
}

void config_free(config_t * config) {
	free(config->format);

	free(config);
}
