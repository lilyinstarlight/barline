BIN=barline

SRC=barline.c config.c format.c bspwm.c widget.c batt.c cpu.c mem.c temp.c text.c timedate.c vol.c win.c wlan.c work.c

CC?=cc
CFLAGS+=-Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-format-zero-length -std=c11 -D_DEFAULT_SOURCE
LDLIBS=-lasound -lxcb

SRCDIR=src

PREFIX?=/usr/local
BINDIR?=$(PREFIX)/bin
