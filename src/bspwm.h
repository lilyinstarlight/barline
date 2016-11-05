#ifndef BSPWM_H
#define BSPWM_H
#include <stdio.h>

int bspwm_subscribe(const char * events);
int bspwm_readline(int bspwmfd, char * buf, size_t size);
int bspwm_next(char ** report);
int bspwm_monitor(char ** report, const char * monitor);
#endif
