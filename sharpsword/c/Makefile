#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

CC	=  gcc
CFLAGS	+= -g -Wall -m32 -std=gnu99 -D__LINUX

all: sudorax

sudorax: sudorax.c
	${CC} ${CFLAGS} -o $@ $<

clean:
	rm -f *.o
clobber: clean
	rm -f sudorax
cl: clobber
