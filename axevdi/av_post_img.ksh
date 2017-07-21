#!/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

dpostimg=${1?"*** dir saving QR Code png files"}
interval=${2:-"5"}

cd $dpostimg
for img in $(ls -1 *.png); do
	eog $img > /dev/null 2>&1 &
	pid=$!
	sleep $interval
	kill -9 $pid
done
