#!/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

dpollimg=${1?"*** dir to save screenshot as png file"}
interval=${2:-"1"}

i=0
while :; do
	(( i += 1 ))
	id=$(printf "%09d" $i)
	f_img=$dpollimg/foo${id}.png
	shutter -f -o $f_img -C -n -e #-d $interval
	sleep $interval
done
