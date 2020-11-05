#!/bin/bash
#
# Copyright (C) 2014, 2020, Vector Li (idorax@126.com). All rights reserved.
#

#
# This is script is to delete all blank lines endswith
# spaces (' ' or '\t' mixed)
# o sed: sed -i "s/[[:space:]]*$//g" <file>
# o vim: %s/\s\+$//g
#

NAME=$(basename $0)

S_CAT=cat
S_SED=sed
S_DOS2UNIX=/usr/bin/dos2unix

if [[ ! -x $S_DOS2UNIX ]]; then
	echo "Oops, $S_DOS2UNIX not found" >&2
	exit 1
fi

dir2backup=/tmp/.$NAME/$(date +"%m%d")
[[ ! -d $dir2backup ]] && mkdir -p $dir2backup

for file in $@; do
	[[ ! -f $file ]] && continue

	fbk=$dir2backup/$(basename $file).backup
	fr1=$dir2backup/$(basename $file).cat1
	fr2=$dir2backup/$(basename $file).cat2

	printf "Start to deal with file $file\n"
	printf "    1# backup $file to $fbk\n"
	cp $file $fbk
	printf "     # dos2unix $file\n"
	$S_DOS2UNIX $file
	printf "    2# save $file via $S_CAT -A as $fr1\n"
	$S_CAT -A $file > $fr1
	printf "    3# delete lines endswith spaces (' ' or '\t' mixed)\n"
	$S_SED -i "s/[[:space:]]*$//g" $file
	printf "    4# save $file via gcat -A again as $fr2\n"
	$S_CAT -A $file > $fr2

	printf "    5# diff $fr1 $fr2\n"
	if [[ -x ~/bin/colordiff ]]; then
		diff=~/bin/colordiff
	else
		diff=diff
	fi
	$diff -u $fr1 $fr2 | $S_SED 's/^/       /g'
	printf "\n"
done

rm -f $f_dos2unix
exit 0
