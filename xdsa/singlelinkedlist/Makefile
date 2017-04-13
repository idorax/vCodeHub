#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -m32 -std=c99
TARGETS	= reverse \
	  reverse2 \
	  test

all: ${TARGETS}

reverse: reverse.c
	${CC} ${CFLAGS} -o $@ $<

reverse2: reverse2.c
	${CC} ${CFLAGS} -o $@ $<

test: test.ksh
	cp $< $@
	chmod +x $@

clean:
	rm -f *.o

clobber: clean
	rm -f ${TARGETS}
cl: clobber
