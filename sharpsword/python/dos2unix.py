#!/usr/bin/python
#
# Copyright (C) 2014, 2017, Vector Li (idorax@126.com). All rights reserved.
#

'''
Linux doesn't have dos2unix command installed.
We write a script to do it.

There is a one-line script, e.g.
    cat <f.dos> | \
        python -c "import sys; \
        sys.stdout.write(sys.stdin.read().replace('\r\n', '\n'))" > <f.unix>

But I don't use it because sometimes the last line doesn't end with '\n',
I'll append '\n' to make 'cat <file>' happy (Note: this not is standard
dos2unix behavior but my favor as I'm too lazy to edit text file manually).
'''

import sys

def dos2unix(fd):
	'''Do dos2unix line by line.

	If line has '\r\n', replace '\r\n' with '\n';
	If line doesn't endswith('\n'), append '\n'.  (2)

	NOTE: (2) is not standard dos2unix behavior, it's just my favor as
	      I'm too lazy to edit text file manually.
	'''
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
		sys.stderr.write("Oops, fail to open file `%s'\n" % filename)
		sys.stderr.write("{0}\n".format(e))
		return 1

	dos2unix(fd)

	fd.close()

	return 0

if __name__ == '__main__':
	argv = sys.argv
	argc = len(argv)
	sys.exit(main(argc, argv))
