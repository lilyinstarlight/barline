#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"

config_t config_default = { .fmt = "no format in config", .poll = 1 };

config_t * config_load(const char * filename) {
	FILE * file;
	int line;
	int ret;

	char option[16];
	char buf[1024];

	config_t * config;

	file = fopen(filename, "r");

	if(file == NULL) {
		fprintf(stderr, "error: cannot load configuration file (%s)\n", filename);
		return NULL;
	}

	config = malloc(sizeof(config_t));
	memcpy(config, &config_default, sizeof(config_t));

	line = 0;
	while(!feof(file)) {
		ret = fscanf(file, "%15s %1023[^\n]", option, buf);

		line++;

		if(ret == EOF)
			break;
		else if(ret != 2) {
			fscanf(file, "%*[^\n]");
			fprintf(stderr, "warning: invalid syntax (line %d) ignored\n", line);
			continue;
		}

		switch(option[0]) {
			case 'f':
				config->fmt = malloc(strlen(buf) + 1);
				strcpy(config->fmt, buf);
				break;
			case 'p':
				ret = sscanf(buf, "%u", &(config->poll));
				if(ret != 1)
					fprintf(stderr, "warning: invalid poll (%s) ignored\n", buf);
				break;
			default:
				fprintf(stderr, "warning: invalid configuration option (%s) ignored\n", option);
				break;
		}
	}

	return config;
}

void config_free(config_t * config) {
	free(config->fmt);

	free(config);
}
