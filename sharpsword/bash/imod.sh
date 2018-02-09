#!/bin/bash
#
# Copyright (C) 2014, 2018, Vector Li (idorax@126.com). All rights reserved.
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

dir2backup=/tmp/.$NAME/$(date +"%m%d")
[[ ! -d $dir2backup ]] && mkdir -p $dir2backup

f_dos2unix=/tmp/.$NAME/dos2unix
cat > $f_dos2unix << EOF
#!/usr/bin/python

import sys

def dos2unix(fd):
	for line in fd.readlines():
		line = line.replace('\r\n', '\n')
		if not line.endswith('\n'):
			line += '\n'
		sys.stdout.write(line)

def main(argc, argv):
	if argc != 2:
		sys.stderr.write("Usage: {0} <file>\n".format(argv[0]))
		return 1

	filename = argv[1]
	try:
		fd = open(filename, "rb")
	except IOError, e:
		return 1

	dos2unix(fd)

	fd.close()

	return 0

if __name__ == '__main__':
	argv = sys.argv
	argc = len(argv)
	sys.exit(main(argc, argv))

EOF

chmod 0555 $f_dos2unix

for file in $@; do
	[[ ! -f $file ]] && continue

	fbk=$dir2backup/$(basename $file).backup
	fr1=$dir2backup/$(basename $file).cat1
	fr2=$dir2backup/$(basename $file).cat2

	printf "Start to deal with file $file\n"
	printf "    1# backup $file to $fbk\n"
	cp $file $fbk
	printf "     # dos2unix $file\n"
	$f_dos2unix $fbk > $file
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
