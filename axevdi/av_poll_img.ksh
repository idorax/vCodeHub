#!/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

FILE=${.sh.file}
NAME=$(basename $FILE)
CDIR=$(dirname  $FILE)

dpollimg=${1?"*** dir to save screenshot as png file"}
xoutfile=${2:-"$dpollimg/foo.out"}
interval=${3:-"0"}

retry_times=${RETRY_TIMES:-"30"}

[[ ! -d $dpollimg ]] && mkdir -p $dpollimg

i=0
while :; do
	(( i += 1 ))
	id=$(printf "%09d" $i)
	f_img=$dpollimg/foo${id}.png
	shutter -f -o $f_img -C -n -e > /dev/null 2>&1
	s=$($CDIR/av_decode $f_img)
	rm -f $f_img
	if [[ -n "$s" ]]; then
		count=0
		echo "$s" >> $xoutfile
		echo $s
	else
		echo "Oops, catch nothing ..." >&2
		(( count += 1 ))
		if (( count > $retry_times )); then
			echo "Well, catch nothing after try $count times, " \
			     "good bye ..."
			exit 1
		fi
	fi

	(( interval > 0 )) && sleep $interval
done
