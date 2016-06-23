#ifndef CONFIG_H
#define CONFIG_H
typedef struct {
	char * fmt;
	unsigned int poll;
} config_t;

config_t * config_load(const char * filename);
void config_free(config_t * config);
#endif
