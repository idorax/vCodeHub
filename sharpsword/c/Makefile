#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	=  gcc
CFLAGS	+= -g -Wall -m32 -std=gnu99 -D__LINUX

all: sudorax \
	test

sudorax: sudorax.c
	${CC} ${CFLAGS} -o $@ $<

test: test.sh
	cp $< $@ && chmod +x $@

clean:
	rm -f *.o
clobber: clean
	rm -f sudorax
	rm -f test
cl: clobber
