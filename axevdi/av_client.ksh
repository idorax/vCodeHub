#!/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

FILE=${.sh.file}
NAME=$(basename $FILE)
CDIR=$(dirname  $FILE)
TMPDIR=${TMPDIR:-"/tmp"}
PNGDIR=$TMPDIR/${NAME}PNGDIR

function cleanup
{
	pkill shutter
	pkill av_poll_img
	rm -rf $PNGDIR
}

outfile=${1?"*** what file to save the output"}
interval=${2:-"0"}

trap "cleanup" EXIT

#
# 1. keep polling until catch nothing more than N (say 30) times
#
clear
[[ ! -d $PNGDIR ]] && mkdir -p $PNGDIR
export CAPMODE="S" # don't have to capture the full screen
$CDIR/av_poll_img $PNGDIR $outfile $interval

#
# 2. use out tools to handle the output
#
tmpfile=$PNGDIR/${NAME}.tmp.out
$CDIR/av_uniq $outfile > $tmpfile
$CDIR/av_verify $tmpfile
if (( $? == 0 )); then
	$CDIR/av_amend $tmpfile > $outfile
	rm -f $tmpfile

	xoutfile=${outfile}.X
	awk '{print $NF}' $outfile | base64 -d > $xoutfile
	print
	print "Nice, $outfile is available!!"
	print "      please review $xoutfile"
	exit 0
else
	print "Oops, $outfile is not complete" >&2
	print "      please check it manually" >&2
	exit 1
fi
