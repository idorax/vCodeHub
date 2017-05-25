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

runit ./sudorax -e "Hello world!"
s=$(./sudorax -l 3 "Welcome to China!!")
runit ./sudorax -d $s        # should pass
runit "sudo ./sudorax -d $s" # should fail
runit sleep 5
runit ./sudorax -d $s        # should fail

s=$(./sudorax -m 9001 -e "idorax")
runit ./sudorax -m 9001 -d $s # should pass
runit ./sudorax -m 8002 -d $s # should fail

runit "./sudorax -e -f ./sudorax.c > /tmp/foo.out"
runit "./sudorax -d -f /tmp/foo.out > /tmp/foo.c"
runit "diff ./sudorax.c /tmp/foo.c && echo OKAY || echo Oops"
rm -f /tmp/foo.out /tmp/foo.c

exit 0
