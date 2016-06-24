BIN=barline

SRC=barline.c config.c format.c widget.c batt.c cpu.c mem.c temp.c timedate.c vol.c widget.c win.c wlan.c work.c

CC?=cc
CFLAGS+=-Wall -Wextra -Wpedantic -Werror -std=c11 -D_DEFAULT_SOURCE -ggdb
LDLIBS=-lasound

SRCDIR=src

PREFIX?=$(DESTDIR)/usr/local
BINDIR?=$(PREFIX)/bin
