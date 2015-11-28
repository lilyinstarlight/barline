#ifndef CONFIG_H
#define CONFIG_H
typedef struct config {
	char * format;
} config_t;

config_t * config_load(const char * filename);
void config_free(config_t * config);
#endif
