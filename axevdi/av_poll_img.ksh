#!/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

FILE=${.sh.file}
NAME=$(basename $FILE)
CDIR=$(dirname  $FILE)
TMPDIR=${TMPDIR:-"/tmp"}

dpollimg=${1?"*** dir to save screenshot as png file"}
xoutfile=${2:-"$dpollimg/foo.out"}
interval=${3:-"0"}
capmode=${CAPMODE:-"F"} # can be F or S; F: "-f", S: "-s X,Y,W,H"

retry_times=${RETRY_TIMES:-"30"}

[[ ! -d $dpollimg ]] && mkdir -p $dpollimg

if [[ $capmode == "F" ]]; then
	shutteropt="-f"
	print "Note the full screen will be captured ..."
else
	shutteropt="-s"

	f_XY=$TMPDIR/.av_clickXY
	print "1# move mouse to determine where you start to cap ..."
	$CDIR/av_click -i 1 -c 0 -o $f_XY
	X0=$(cat $f_XY | tr ',' ' ' | awk '{print $1}')
	Y0=$(cat $f_XY | tr ',' ' ' | awk '{print $2}')

	print "2# move mouse to determine where you end to cap ..."
	$CDIR/av_click -i 1 -c 0 -o $f_XY
	X1=$(cat $f_XY | tr ',' ' ' | awk '{print $1}')
	Y1=$(cat $f_XY | tr ',' ' ' | awk '{print $2}')

	W=$(( X1 - X0 ))
	H=$(( Y1 - Y0 ))
	shutteropt+=" $X0,$Y0,$W,$H"
	print "Note block ($X0,$Y0,$W,$H) will be captured ..."
fi

print "3# move mouse to determine where you want to click ..."
$CDIR/av_click -i 1 -c 0 -o $f_XY
X2=$(cat $f_XY | tr ',' ' ' | awk '{print $1}')
Y2=$(cat $f_XY | tr ',' ' ' | awk '{print $2}')

t_start=$(date)
s_prev=""
i=0
while :; do
	(( i += 1 ))
	id=$(printf "%09d" $i)
	f_img=$dpollimg/foo${id}.png
	shutter $shutteropt -o $f_img -C -n -e > /dev/null 2>&1
	s=$($CDIR/av_decode $f_img)
	rm -f $f_img
	if [[ -n "$s" ]]; then
		count=0
		[[ $s == $s_prev ]] && continue
		print "$s" >> $xoutfile
		s_prev="$s"
		print $s
		s=""

		$CDIR/av_click_once $X2 $Y2 > /dev/null 2>&1
	else
		print "Oops, catch nothing ..." >&2
		(( count += 1 ))
		if (( count > $retry_times )); then
			t_end=$(date)
			print "Start: $t_start ..."
			print "End :  $t_end ..."
			print "Well, catch nothing after try $count times, " \
			      "good bye ..."
			exit 1
		fi
	fi

	(( interval > 0 )) && sleep $interval
done
