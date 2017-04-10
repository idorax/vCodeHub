#!/usr/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

istart=${1:-"1"}
iend=${2:-"8"}
nums=$(seq $istart $iend)
loop=$(( iend - istart + 3 ))

for (( i = 0; i < loop; i++ )); do
	print "\$ ./reverse $i $(print $nums)"
	./reverse $i $(print $nums)
	print ""
done
