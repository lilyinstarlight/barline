#ifndef BSPWM_H
#define BSPWM_H
#include <stdio.h>
#include <sys/types.h>

enum bspwm_domain { NODE, DESKTOP, MONITOR };

int bspwm_subscribe(const char * events);
void bspwm_id(enum bspwm_domain domain, const char * name, char * buf, size_t size);
int bspwm_readline(int bspwmfd, char * buf, size_t size);
int bspwm_next(char ** report);
int bspwm_monitor(char ** report, const char * monitor);
int bspwm_check(pid_t child_pid);
void bspwm_close();
#endif
