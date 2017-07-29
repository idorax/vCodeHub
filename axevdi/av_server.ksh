#!/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

FILE=${.sh.file}
NAME=$(basename $FILE)
CDIR=$(dirname  $FILE)
TMPDIR=${TMPDIR:-"/tmp"}

src_file=${1?"*** what file to post?"}
interval=${2:-"10"}
post_mode=${POST_MODE:-"static"}

echo "#1 base64 $src_file ..."
src_file_b64=$TMPDIR/${NAME}.base64.out
base64 $src_file | cat -n > $src_file_b64

echo "#2 encode ..."
PNGDIR=$TMPDIR/${NAME}PNGDIR
rm -rf $PNGDIR && mkdir -p $PNGDIR
$CDIR/av_encode_dynamic $PNGDIR/foo:9 $src_file_b64
rm -f $src_file_b64

trap "rm -rf $PNGDIR && pkill eog" EXIT

echo "#3 post ..."
cd $PNGDIR
pkill eog
count=$(ls -1 $PNGDIR/*.png | wc -l)
(( count *= 2 ))
eog $PNGDIR/*.png > /dev/null 2>&1 &
pid=$!

if [[ $post_mode == "static" ]]; then
	print "In static mode ..."

	for (( i = 0; i < count; i++ )); do
		ps -p $pid > /dev/null 2>&1 || exit 0
		sleep $interval
	done

	ps -p $pid > /dev/null 2>&1 && kill -9 $pid
else
	print "In dynamic mode ..."

	$CDIR/av_click -i $interval -c $count -d 60 -B $pid -C
fi

exit 0
