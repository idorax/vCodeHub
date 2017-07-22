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

echo "#1 base64 $src_file ..."
src_file_b64=$TMPDIR/${NAME}.base64.out
base64 $src_file | cat -n > $src_file_b64

echo "#2 encode ..."
PNGDIR=$TMPDIR/${NAME}PNGDIR
rm -rf $PNGDIR && mkdir -p $PNGDIR
$CDIR/av_encode $PNGDIR/foo:9 $src_file_b64

trap "rm -rf $PNGDIR && pkill eog" EXIT

clear
echo "#3 post ..."
$CDIR/av_post_img $PNGDIR $interval
