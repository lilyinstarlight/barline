NAME=barline

SRC=barline.c config.c data.c format.c util.c

CC?=cc
CFLAGS+=-Wall -Wextra -Wpedantic -Werror -std=c99 -D_DEFAULT_SOURCE -ggdb
LDLIBS=-lasound

SRCDIR=src

PREFIX?=$(DESTDIR)/usr/local
BINDIR?=$(PREFIX)/bin
