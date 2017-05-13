#!/usr/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

function runit
{
	print "$(id -un)@$(hostname)\$ $*"
	eval "$@"
	print
}

runit ./foo
runit ./foo 3
runit ./foo -R 3

for e in 0x1001 0x1002 0x1003 0x1004; do
	runit ./foo -d $e 3
done

for e in 0x1001 0x1002 0x1003 0x1004; do
	runit ./foo -i 0x7fff -d $e 3
done

for e in 0x1001 0x1002 0x1003 0x1004; do
	runit ./foo -I 0x7fff -d $e 3
done
