#!/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

FILE=${.sh.file}
NAME=$(basename $FILE)
CDIR=$(dirname  $FILE)
TMPDIR=${TMPDIR:-"/tmp"}

dpostimg=${1?"*** dir saving QR Code png files"}
interval=${2:-"10"} # 10s is proper, av_poll_img can catch at least 2 shots

trap "touch /tmp/dadyisback && pkill eog" EXIT

cd $dpostimg
pkill eog
count=$(ls -1 $dpostimg/*.png | wc -l)
(( count *= 2 ))
eog $dpostimg/*.png > /dev/null 2>&1 &
$CDIR/av_click -i $interval -c $count -d 60 -C
