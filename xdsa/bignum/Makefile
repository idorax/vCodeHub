#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -std=c99

TARGETS	= foo bar

all: $(TARGETS)

foo: foo.o libbignum.o
	$(CC) $(CFLAGS) -o $@ $^

foo.o: foo.c
	$(CC) $(CFLAGS) -c $<

libbignum.o: libbignum.c libbignum.h
	$(CC) $(CFLAGS) -c $<

bar: bar.py
	cp $< $@ && chmod +x $@

clean:
	rm -f *.o
clobber: clean
	rm -f $(TARGETS)
cl: clobber
