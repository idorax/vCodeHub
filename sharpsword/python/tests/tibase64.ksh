#!/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

FILE=${.sh.file}
NAME=$(basename $FILE)
CDIR=$(dirname $FILE)

function runit
{
	echo "$(id -un)@$(hostname)\$ $*"
	eval "$*"
	typeset -i rc=$?
	echo
	return $rc
}

tpwd=${1:-"l1admin"}
tbin=${2:-"/bin/ls"}
tbin2=/tmp/$(basename $tbin)
ftmp1=/tmp/a.out
ftmp2=/tmp/b.out
trap "rm -f $ftmp1 $ftmp2 $tbin2" EXIT

export PATH=$(dirname $CDIR):$PATH
runit "whence -p ibase64"

runit "ibase64 $tbin > $ftmp1"
runit "base64  $tbin > $ftmp2"
runit "diff $ftmp1 $ftmp2 && echo OKAY || echo Oops"
runit "ibase64 -d $ftmp1 > $tbin2"
runit "md5sum $tbin $tbin2"
runit "base64  -d $ftmp2 > $tbin2"
runit "md5sum $tbin $tbin2"

runit "ibase64 -p \"$tpwd\" -e $tbin > $ftmp1"
runit "ibase64 -p \"$tpwd\" -d $ftmp1 > $tbin2"
runit "md5sum $tbin $tbin2"
runit "diff $tbin $tbin2 && echo OKAY || echo Oops"

runit "ibase64 -p \"$tpwd\" -e $tbin > $ftmp1"
runit "ibase64 -p \"passw0rd\" -d $ftmp1 > $tbin2"
runit "md5sum $tbin $tbin2"
diff $tbin $tbin2 && echo "OKAY" || echo "Oops"
