#
# Copyright (C) 2017, 2018, Vector Li (idorax@126.com). All rights reserved.
#

CC	= gcc
CFLAGS	= -g -Wall -std=gnu99
INCS	=

TARGETS	= kmp

all: ${TARGETS}

kmp: kmp.o
	${CC} ${CFLAGS} -o $@ $^

kmp.o: kmp.c
	${CC} ${CFLAGS} -c $< ${INCS}

clean:
	rm -f *.o
clobber: clean
	rm -f ${TARGETS}
cl: clobber
