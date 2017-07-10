#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	=  gcc
CFLAGS	+= -g -Wall -std=gnu99 -D__LINUX

all:	sudorax \
	binhex \
	test

sudorax: sudorax.c
	${CC} ${CFLAGS} -o $@ $<

binhex: binhex.c
	${CC} ${CFLAGS} -o $@ $<

test: test.sh
	cp $< $@ && chmod +x $@

clean:
	rm -f *.o
clobber: clean
	rm -f sudorax
	rm -f binhex
	rm -f test
cl: clobber
