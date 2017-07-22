#!/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

dpostimg=${1?"*** dir saving QR Code png files"}
interval=${2:-"10"} # 10s is proper, av_poll_img can catch at least 2 shots

cd $dpostimg
for img in $(ls -1 *.png); do
	pkill eog
	eog $img > /dev/null 2>&1 &
	pid=$!
	sleep $interval
	ps -p $pid > /dev/null 2>&1 & kill -9 $pid
done
