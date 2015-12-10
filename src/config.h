typedef struct config {
	char * format;
	unsigned int interval;
} config_t;

config_t * config_load(const char * filename);
void config_free(config_t * config);
