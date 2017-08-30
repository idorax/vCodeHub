#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -std=c99

TARGETS	= foo \
	  foo_fac \
	  foo_pow \
	  foo_shl \
	  foo_shr \
	  bar \
	  bar_fac

all: $(TARGETS)

foo: foo.o libbignum.o
	$(CC) $(CFLAGS) -o $@ $^

foo.o: foo.c
	$(CC) $(CFLAGS) -c $<

foo_fac: foo_fac.o libbignum.o
	$(CC) $(CFLAGS) -o $@ $^

foo_fac.o: foo_fac.c
	$(CC) $(CFLAGS) -c $<

foo_pow: foo_pow.o libbignum.o
	$(CC) $(CFLAGS) -o $@ $^

foo_pow.o: foo_pow.c
	$(CC) $(CFLAGS) -c $<

foo_shl: foo_shl.o libbignum.o
	$(CC) $(CFLAGS) -o $@ $^

foo_shl.o: foo_shl.c
	$(CC) $(CFLAGS) -c $<

foo_shr: foo_shr.o libbignum.o
	$(CC) $(CFLAGS) -o $@ $^

foo_shr.o: foo_shr.c
	$(CC) $(CFLAGS) -c $<

libbignum.o: libbignum.c libbignum.h
	$(CC) $(CFLAGS) -c $<

bar: bar.py
	cp $< $@ && chmod +x $@

bar_fac: bar_fac.py
	cp $< $@ && chmod +x $@

clean:
	rm -f *.o
clobber: clean
	rm -f $(TARGETS)
cl: clobber
