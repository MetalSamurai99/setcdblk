# Makefile for setcdblk	-*-Makefile-*-
# Copyright (C) 2024 Kevin Davidson metalsamurai@me.com
# $Header$
# $Log$

CC = gcc
CFLAGS = -g -O
MAKE = make
COMPRESS = gzip
INSTALL = /usr/ucb/install

OBJS = setcdblk.o
SRCS = $(OBJS:.o=.c)
LIBS =

RUBBISH = $(OBJS) *.log *.aux *.tp *.vr *.cp *.fn *.ky *.pg

DISTFILES = $(SRCS) Makefile cdblk

all: setcdblk
	@echo All done.

setcdblk: $(OBJS)
	$(CC) -o setcdblk $(OBJS) $(LIBS)

install: all
	$(INSTALL) -c -m 755 setcdblk /usr/sbin
	$(INSTALL) -c -m 755 cdblk /etc/init.d
	ln -s ../init.d/cdblk /etc/rc2.d/S92cdblk

clean:
	-rm -f $(RUBBISH)

dist:
	echo setcdblk-`sed -e '/Revision/!d' \
			-e 's/[^0-9.]*\([0-9.]*\).*/\1/' -e q setcdblk.c` > .fname
	-rm -rf `cat .fname`
	mkdir `cat .fname`
	dst=`cat .fname`; for f in $(DISTFILES); do \
		ln $$f $$dst/$$f || { echo copying $$f; \
		  cp -p $$f $$dst/$$f ; } \
	done
	tar -cf `cat .fname`.tar `cat .fname`
	$(COMPRESS) `cat .fname`.tar
	-rm -rf `cat .fname` .fname
