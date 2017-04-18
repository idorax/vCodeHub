#!/usr/bin/bash
#
# Copyright (C) 2014, 2017, Vector Li (idorax@126.com). All rights reserved.
#

#
# This is script is to delete all blank lines
# endswith spaces (' ' or '\t' mixed)
# it is the same as vim :%s/\s\+$//g
#

#
# Example: on Solaris
#
# 1. delete all blank lines including multiple ' ' or '\t' mixed
# 	gsed -i "/^[[:space:]]*$/d" <file>
#
# 2. delete all blank lines
# 	gsed -i "/^$/d" <file>
#
# 3. delete all spaces endswith of lines
# 	gsed -i "s/[[:space:]]*$//g" <file>
#     = vim :%s/\s\+$//g
#

NAME=$(basename $0)

if [[ $(uname) == SunOS ]]; then
	S_CAT=gcat
	S_SED=gsed
else # Linux
	S_CAT=cat
	S_SED=sed
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
	dos2unix $fbk > $file
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

exit 0
