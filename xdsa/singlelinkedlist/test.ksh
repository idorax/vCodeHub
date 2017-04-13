#!/usr/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

tbin=${1:-"./reverse"}
shift
istart=${1:-"1"}
iend=${2:-"8"}
nums=$(seq $istart $iend)
loop=$(( iend - istart + 3 ))

for (( i = 0; i < loop; i++ )); do
	print "\$ $tbin $i $(print $nums)"
	$tbin $i $(print $nums)
	print ""
done
