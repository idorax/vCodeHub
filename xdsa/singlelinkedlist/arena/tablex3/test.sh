#!/bin/bash
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

function runit
{
	echo "$(id -un)@$(hostname)\$ $*"
	eval "$@"
	echo
}

runit ./foo -h
runit ./foo -x
runit ./foo
runit ./foo -a
runit ./foo -m
runit ./foo -l
runit ./foo -f

runit ./foo -c
runit ./foo -c 99
for cid in $(cat data/courses.dat | awk '{print $2}' | grep -v '#'); do
	runit ./foo -c $cid
done

runit ./foo -s
runit ./foo -s 1234
for sid in $(cat data/students.dat | awk '{print $1}' | grep -v '#'); do
	runit ./foo -s $sid
done

runit ./foo -a -c 1
runit ./foo -a -s 2
runit ./foo -c 1 -s 5

rm -rf /tmp/data && cp -r data /tmp/data
runit ./foo -d /tmp/data -o /tmp/foo.out
runit cat /tmp/foo.out && rm -rf /tmp/data

exit 0
